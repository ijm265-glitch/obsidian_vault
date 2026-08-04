SMT 공정 개선 멀티모달 데이터 - AI Hub
[[YOLO 데이터 전처리]]

```python
from ultralytics import YOLO

model = YOLO('yolov8n.pt')

results = model.train(
    data='C:/Users/ljm79/Onedrive/Desktop/python/yolo_train/18.SMT 공정 개선 멀티모달 데이터/yolo_dataset/data.yaml',
    epochs=50,
    patience=10,
    imgsz=512, # resize
    batch=16,
    device=0
)
```
![[Pasted image 20260804144358.png]]
학습 시작시 다음과 같은 runs/detect/train 디렉토리와 다음과 같은 파일이 생성됨
- **`best.pt`:** Validation 성능(mAP)이 **가장 높았던 시점의 모델**
- **`last.pt`:** **가장 마지막 에포크시점의 모델**
`args.yaml`: 학습 시작시 넘겨준 파라미터와 데이터 증강을 위한 기본 하이퍼파라미터 설정이 적혀있는 파일
`labels.jpg`: 
![[Pasted image 20260804145138.png]]

### 1. 왼쪽 위: 클래스별 객체 수 (Classes Distribution)

- **내용:** 0번부터 31번까지 **각 클래스(0~31)별로 라벨이 몇 개(instances)씩 존재하는지** 보여주는 막대그래프입니다.
    
- **해석:** 특정 막대(예: 16번, 21번 클래스 등)가 유독 높고, 어떤 건 매우 낮죠? 데이터셋에 클래스 불균형(Class Imbalance)이 어느 정도 존재하는지 직관적으로 확인할 수 있습니다.
    

### 2. 오른쪽 위: 바운딩 박스 형태 시각화 (Bounding Box Overlay)

- **내용:** 데이터셋에 존재하는 **바운딩 박스들의 폼(가로/세로 비율 및 크기)을 겹쳐서** 시각화한 모습입니다.
    
- **해석:** 중앙을 기준으로 정 정사각형에 가까운 박스부터 얇고 긴 직사각형 박스까지 어떤 비율의 객체들이 포함되어 있는지 대략적인 형태를 파악할 수 있습니다.
    

### 3. 왼쪽 아래: 객체 중심 위치 히트맵 ($x, y$ Position Heatmap)

- **내용:** 이미지 안에서 **객체(바운딩 박스)의 중심점(Center Point)이 주로 어디에 위치하는지** 보여주는 2D 히트맵입니다. ($0.5, 0.5$가 이미지 정중앙)
    
- **해석:** 정중앙 부근($x=0.5, y=0.5$)에 파란색 입자가 진하게 집중되어 있는 것으로 보아, 라벨링된 객체들이 주로 **이미지 중앙 부근에 조밀하게 배치**되어 있음을 뜻합니다.
    

### 4. 오른쪽 아래: 객체의 너비와 높이 분포 (Width & Height Distribution)

- **내용:** 객체의 **가로 크기(width)와 세로 크기(height)** 비율 분포를 보여주는 히트맵입니다. ($0.2$는 이미지 전체 크기의 $20\%$ 수준)
    
- **해석:** 파란 점이 $width \approx 0.2, height \approx 0.2$ 부근에 강하게 모여 있죠? 데이터셋에 포함된 대부분의 객체(납땜/부품 영역)가 **전체 이미지 크기의 $10 \sim 20\%$ 내외를 차지하는 소형~중형 크기의 객체**라는 것을 의미합니다.





`train_batch.jpg`:

![[Pasted image 20260804145434.png]]

### 1. 라벨 변환(COCO $\rightarrow$ YOLO)의 성공 여부 확인

- 전처리한 JSON 라벨이 YOLO 전용 `.txt` 파일로 제대로 변환되었는지 눈으로 직접 확인할 수 있는 가장 확실한 방법입니다.
    
- 이미지에 보이는 알록달록한 네모 박스(Bounding Box)와 상단의 숫자(Class ID)는 사람이 입력해 준 실제 정답(Ground Truth)을 나타냅니다.
    
- **박스가 납땜 부위에 딱 맞게 둘러쳐져 있다면 전처리가 100% 성공적으로 끝난 것입니다!**
    
### 2. 데이터 증강(Mosaic Augmentation) 적용 확인

- 사진을 잘 보시면 이미지 4장이 서로 이어 붙여져 1장의 콜라주 사진처럼 되어 있는 것을 볼 수 있습니다.
    
- 이건 YOLO의 대표적인 학습 기법인 모자이크 데이터 증강(Mosaic Augmentation)이 정상적으로 적용된 모습입니다.
    
- 여러 이미지를 섞어 학습시킴으로써 모델이 다양한 위치와 크기의 객체를 더 잘 찾도록 훈련시킵니다.
    
### 3. 배치 단위 샘플링 (`train_batch0`, `1`, `2`)

- 모델이 학습 초기 첫 몇 개의 배치(Batch)에 대해 위와 같은 가시화 사진을 `train_batch0.jpg`, `train_batch1.jpg`, `train_batch2.jpg` 세 개 정도로 찍어서 보관해 둔 것입니다.