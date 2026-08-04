## 1. 기초 개념

- **Precision (정밀도):** 모델이 양성이라고 예측한 것 중 실제로 양성인 것
    
    $$\text{Precision} = \frac{\text{실제 양성 수}}{\text{모델이 양성이라고 예측한 수}}$$
    
- **Recall (재현율):** 실제 양성 수 중 모델이 올바르게 양성으로 예측한 수
    
    $$\text{Recall} = \frac{\text{진짜 맞춘 결함 수}}{\text{실제 존재하는 총 결함 수}}$$
    

> 객체 인식 문제는 대표적인 불균형 문제이기 때문에 classification threshold를 낮추면 Recall은 올라가지만 Precision은 떨어지고
> threshold를 높이면 Recall은 내려가지만 Precision이 올라가는 관계가 형성됨


## 2. AP (Average Precision)

Precision과 Recall 중 하나만 보면 모델의 전체 성능을 왜곡해서 평가할 수 있음

그래서 탐지 기준(Threshold)을 $0.0$부터 $1.0$까지 조금씩 바꾸면서 Precision-Recall 곡선(PR Curve)을 그립니다.

- **AP (Average Precision):** 이 PR 곡선 아래의 면적(Area Under Curve)을 의미합니다.

- Precision과 Recall이 둘 다 높아야 좋은 모델이므로 좋은 평가기준이 될 수 있음

## 3. mAP (Mean Average Precision)

객체 탐지 모델은 여러개의 클래스를 탐지할 수 있도록 설계되었음

따라서 각각의 클래스에 따른 AP Curve가 그려지게 되고 각각의 AUC (Area Under Curve)값을 얻을 수 있음

이 클래스 별 AP 값들의 평균이 **mAP**임


## 4. mAP50, mAP50-95

 **위치 맞춤의 정밀도(IoU)** 기준을 의미함

- **IoU (Intersection over Union):** 정답 상자와 모델이 그린 상자가 얼마나 겹치는 지 나타내는 비율 ($0 \sim 1$)
    
- **mAP50 (또는 mAP@0.5):** 정답 상자와 $50\%$ 이상만 겹치면 정답으로 인정하고 측정한 mAP
    
- **mAP50-95 (또는 mAP@0.5:0.95):** IoU 기준을 $50\%$에서 $95\%$까지 $5\%$씩 바꾸어가며 측정한 mAP의 평균값


