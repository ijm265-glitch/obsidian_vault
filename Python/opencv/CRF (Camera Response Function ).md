센서에 들어오는 실제 물리적 빛의 양(Radiance, $E$)과 최종 이미지 파일에 기록되는 픽셀 밝기 값(Pixel Value, $Z$)은 1:1 비례(선형) 관계가 아닙니다.

카메라 내부의 ISP(이미지 신호 처리기)는 사람 눈의 특성 및 디스플레이 특성에 맞추기 위해 톤 커브(Gamma Correction 등)를 적용하는 **비선형(Non-linear) 변환**을 거칩니다.

$$Z = f(E \cdot \Delta t)$$

- $Z$: 픽셀 밝기 ($0 \sim 255$)
    
- $E$: 실제 조도 / Radiance (우리가 구하고자 하는 값)
    
- $\Delta t$: 노출 시간 (셔터 스피드)
    
- $f$: 카메라 응답 함수 (**CRF**)

진정한 HDR 이미지를 합성하려면, 서로 다른 셔터 속도로 찍힌 픽셀값들을 비교하기 전에 **카메라가 왜곡시킨 비선형 함수 $f$의 역함수 $f^{-1}$를 구해서 픽셀값들을 '진짜 빛의 양(Radiance)'으로 복원**해야 합니다.

```python
# CRF를 구하는 방법
calibrateDebevec = cv2.createCalibrateDebevec()
responseDebevec = calibrateDebevec.process(images, times)
```