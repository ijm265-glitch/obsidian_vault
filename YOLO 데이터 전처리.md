SMT 공정 개선 멀티모달 데이터
[[os]], [[json]], [[zipfile]], [[glob]], [[pathlib]], [[yaml(PyYAML)]], [[tqdm]]

```python
import os
import json
import zipfile
import glob
from pathlib import Path
import yaml
from tqdm import tqdm

# ==========================================
# 1. 경로 설정
# ==========================================
BASE_DIR = Path(".")  # 현재 작업 디렉토리
DATA_DIR = BASE_DIR / "3.개방데이터" / "1.데이터"
OUTPUT_DIR = BASE_DIR / "yolo_dataset"

# 출력 폴더 생성
for split in ['train', 'val']:
    (OUTPUT_DIR / 'images' / split).mkdir(parents=True, exist_ok=True)
    (OUTPUT_DIR / 'labels' / split).mkdir(parents=True, exist_ok=True)

# ==========================================
# 2. 압축 해제 함수
# ==========================================
def extract_zips(zip_pattern, target_dir):
    zip_files = glob.glob(str(zip_pattern), recursive=True)
    print(f"📦 압축 해제 시작: {len(zip_files)}개 파일 ({zip_pattern})")
    for zip_path in tqdm(zip_files):
        try:
            with zipfile.ZipFile(zip_path, 'r') as zip_ref:
                zip_ref.extractall(target_dir)
        except Exception as e:
            print(f"❌ 압축 해제 실패 [{zip_path}]: {e}")

# ==========================================
# 3. COCO JSON -> YOLO TXT 변환 함수
# ==========================================
def convert_coco_json_to_yolo(json_file_path, output_label_dir, category_id_map):
    with open(json_file_path, 'r', encoding='utf-8') as f:
        coco_data = json.load(f)
    
    # 이미지 ID별 크기 및 파일명 정보 추출
    images_info = {}
    for img in coco_data.get('images', []):
        images_info[img['id']] = {
            'file_name': img['file_name'],
            'width': img['width'],
            'height': img['height']
        }
        
    # 어노테이션 정보 수집 (이미지별로 그룹화)
    img_annotations = {}
    for ann in coco_data.get('annotations', []):
        img_id = ann['image_id']
        if img_id not in img_annotations:
            img_annotations[img_id] = []
        img_annotations[img_id].append(ann)
        
    # TXT 파일 생성
    for img_id, img_info in images_info.items():
        img_w = img_info['width']
        img_h = img_info['height']
        base_name = Path(img_info['file_name']).stem
        txt_path = Path(output_label_dir) / f"{base_name}.txt"
        
        yolo_lines = []
        anns = img_annotations.get(img_id, [])
        for ann in anns:
            cat_id = ann['category_id']
            # COCO category_id -> YOLO class_id (0부터 시작)
            class_id = category_id_map.get(cat_id)
            if class_id is None:
                continue
                
            # COCO bbox: [x_min, y_min, width, height]
            x_min, y_min, bw, bh = ann['bbox']
            
            # YOLO bbox: [x_center, y_center, width, height] (0~1 정규화)
            x_center = (x_min + bw / 2.0) / img_w
            y_center = (y_min + bh / 2.0) / img_h
            norm_bw = bw / img_w
            norm_bh = bh / img_h
            
            # 범위 제한 (0~1)
            x_center = max(0.0, min(1.0, x_center))
            y_center = max(0.0, min(1.0, y_center))
            norm_bw = max(0.0, min(1.0, norm_bw))
            norm_bh = max(0.0, min(1.0, norm_bh))
            
            yolo_lines.append(f"{class_id} {x_center:.6f} {y_center:.6f} {norm_bw:.6f} {norm_bh:.6f}")
            
        with open(txt_path, 'w', encoding='utf-8') as txt_f:
            txt_f.write("\n".join(yolo_lines))

# ==========================================
# 4. 메인 실행 프로세스
# ==========================================
if __name__ == "__main__":
    # 임시 압축 해제용 폴더
    TEMP_DIR = BASE_DIR / "temp_extracted"
    
    # 4-1. 압축 해제 (이미지 & 라벨)
    print("🚀 [1/4] 데이터셋 압축 해제 중...")
    
    # Train 이미지 및 라벨
    extract_zips(DATA_DIR / "Training" / "01.원천데이터" / "TS_이미지데이터_*.zip", OUTPUT_DIR / "images" / "train")
    extract_zips(DATA_DIR / "Training" / "02.라벨링데이터" / "TL_*.zip", TEMP_DIR / "train_labels")
    
    # Validation 이미지 및 라벨
    extract_zips(DATA_DIR / "Validation" / "01.원천데이터" / "VS_이미지데이터_*.zip", OUTPUT_DIR / "images" / "val")
    extract_zips(DATA_DIR / "Validation" / "02.라벨링데이터" / "VL_*.zip", TEMP_DIR / "val_labels")

    # 4-2. 카테고리(클래스) 맵핑 생성
    print("\n🔍 [2/4] 클래스 정보 파싱 중...")
    sample_json = list(TEMP_DIR.glob("**/*.json"))[0]
    with open(sample_json, 'r', encoding='utf-8') as f:
        sample_coco = json.load(f)
        
    categories = sample_coco.get('categories', [])
    # ID 순으로 정렬하여 0부터 부여
    categories = sorted(categories, key=lambda x: x['id'])
    
    category_id_map = {}
    class_names = []
    for idx, cat in enumerate(categories):
        category_id_map[cat['id']] = idx
        class_names.append(cat['name'])
        
    print(f"✅ 총 {len(class_names)}개 클래스 감지됨: {class_names}")

    # 4-3. JSON -> TXT 라벨 변환
    print("\n🔄 [3/4] 라벨 포맷 변환 (JSON -> YOLO TXT) 중...")
    
    for split, label_temp_folder in [('train', TEMP_DIR / 'train_labels'), ('val', TEMP_DIR / 'val_labels')]:
        json_files = list(label_temp_folder.glob("**/*.json"))
        output_label_path = OUTPUT_DIR / "labels" / split
        
        for j_file in tqdm(json_files, desc=f"{split} 변환 중"):
            convert_coco_json_to_yolo(j_file, output_label_path, category_id_map)

    # 4-4. data.yaml 파일 생성
    print("\n📝 [4/4] YOLO data.yaml 생성 중...")
    yaml_data = {
        'path': str(OUTPUT_DIR.resolve()),
        'train': 'images/train',
        'val': 'images/val',
        'names': {idx: name for idx, name in enumerate(class_names)}
    }
    
    yaml_path = OUTPUT_DIR / "data.yaml"
    with open(yaml_path, 'w', encoding='utf-8') as f:
        yaml.dump(yaml_data, f, allow_unicode=True, sort_keys=False)

    print(f"\n🎉 모든 데이터 전처리가 완료되었습니다!")
    print(f"📂 결과 폴더: {OUTPUT_DIR.resolve()}")
    print(f"📄 YAML 파일: {yaml_path.resolve()}")
```