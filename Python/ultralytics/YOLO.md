```python 
from ultralytics import YOLO
model = YOLO("yolo11n.pt") # 모델 객체 생성

# 이미지 또는 프레임 추론
results = model(frame)

annotated_frame = results[0].plot()

plt.imshow(annotated_frame)
```