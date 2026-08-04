```python 
from ultralytics import YOLO
model = YOLO("yolo26n.pt") # 모델 객체 생성

# 이미지 또는 프레임 추론
# frame은 이미지 확장자, ndarray, 배치로 넣어도 작동
results = model(frame)

# 배치의 0번의 결과를 플롯한 이미지를 반환
annotated_frame = results[0].plot()

plt.imshow(annotated_frame)
```

### Results 객체
1. **results\[0\].boxes (바운딩 박스 정보 - 이미지 한 장에서 모델이 인식한 모든 객체들의 묶음)**
	- **`results[0].boxes[0].xyxy`**: 각 물체의 테두리 좌표 `[xmin, ymin, xmax, ymax]`
    
	- **`results[0].boxes[0].conf`**: 신뢰도 점수 (예: `0.95` -> 95% 확신)
    
	- **`results[0].boxes[0].cls`**: 감지된 클래스 번호 (예: `0.0` -> 사람, `2.0` -> 자동차)
2. `results[0].names` (클래스 이름 사전)
	클래스 번호와 이름이 매핑되어 있는 딕셔너리입니다.
	- 예: `{0: 'person', 1: 'bicycle', 2: 'car', ...}`
3. `results[0].plot()` (시각화 함수)

- 원본 이미지 위에 바운딩 박스, 클래스 이름, 신뢰도 점수를 직접 그려서 렌더링된 이미지(ndarray)를 반환해 주는 매우 편리한 함수 

- `cv2.imshow()`로 바로 출력하거나 파일로 저장하기 위해 사용

### 특정 클래스만 인식
1. **YOLO모델을 호출할 때 `classes` 매개변수에 원하는 클래스의 번호 리스트를 넘겨주기**
```python
from ultralytics import YOLO
import cv2

model = YOLO("yolo26n.pt", classes=[0, 2]) 
# 0: person, 2: car
# 따라서 이 모델은 사람과 자동차만 인식하게 됨 

results = model(frame)

annotated_image = results[0].plot()
```

2. **모든 클래스를 인식한 후 특정 클래스만 시각화 하거나 특정 클래스에 대해서만 동작**
```python
from ultralytics import YOLO
import cv2

model = YOLO("yolo26n.pt", classes=[0, 2]) 

results = model(frame) # 감지된 객체 반복 탐색 
for box in results[0].boxes:
	cls_id = int(box.cls[0]) # 클래스 ID (예: 0) 
	conf = float(box.conf[0]) # 확신도 (예: 0.85)
	 
	# 사람이면서(0) 확신도가 50% 이상인 경우에만 로직 실행 
	if cls_id == 0 and conf >= 0.5: 
		print(f"🚨 사람 감지! (확신도: {conf:.2f})") # 스냅샷 찍기 또는 이벤트 저장 실행
```
> 단 특정 클래스만 출력하도록 설정하여도 속도나 정확도가 상승하지 않음
> 내부의 연산을 바꾸는 것이 아니라 마지막 출력 단계에서 필터링 하는 것이기 때문
> 따라서 정보를 보존하는 2번 방법을 추천함 

