```python 
cv2.imread()
cv2.imwrite()
cv2.cvtColor()
cv2.split() : 이미지에서부터 채널을 분리
cv2.merge() : 분리된 채널을 합칩
cv2.resize()
cv2.flip()
cv2.line(img, pt1, pt2, color[, thickness[, lineType[, shift]]])
cv2.circle(img, center, radius, color[, thickness[, lineType[, shift]]])
cv2.rectangle(img, pt1, pt2, color[, thickness[, lineType[, shift]]])
cv2.putText(img, text, org, fontFace, fontScale, color[, thickness[, lineType[, bottomLeftOrigin]]])
cv2.threshold( src, thresh, maxval, type[, dst] ) : 이진화에 사용
cv2.adaptiveThreshold( src, maxValue, adaptiveMethod, thresholdType, blockSize, C[, dst] )
cv2.bitwise_and(), cv2.bitwise_or(), cv2.bitwise_xor(), cv2.bitwise_not()
```

## BGR (Blue, Green, Red)
- 디스플레이나 카메라 센서가 일반적으로 데이터를 다루는 방식
- **단점** : 밝기에 매우 취약하여 같은 빨간색이라도 밝기가 달라지면 R, G, B값이 크게 바뀌어 식별하기 어려움
## HSV (Hue, Saturation, Value)
- **H (Hue, 색상):** 색의 종류 (ex. R, Y, K, B....) **0~179** 범위의 수치로 나타남
- **S (Saturation, 채도):** 색의 선명함/순도 (빛 바랜정도 ) **0~255** 범위의 수치로 나타남
- **V (Value, 명도/밝기):** 밝기 **0~255** 범위의 수치로 나타남
- **장점** : 밝기의 정보가 Value에 치중되어 있어 밝기가 달라져도 H의 값은 일정하게 유지됨 **(Robust)** 

## LAB (CIE L\*a\*b\*)
- **L (Lightness, 밝기/지각 명도):** 인간이 눈으로 느끼는 밝기 정보. 0~255 범위의 수치로 나타남 (0: 검은색, 255: 흰색)
    
- **a (Green-Red Axis, 색상 축 1):** 초록색과 빨간색 사이의 색상 성분. 0~255 범위의 수치로 나타남 (0에 가까울수록 초록, 255에 가까울수록 빨강, 128은 무채색/중립)
- **b (Blue-Yellow Axis, 색상 축 2):** 파란색과 노란색 사이의 색상 성분. 0~255 범위의 수치로 나타남 (0에 가까울수록 파랑, 255에 가까울수록 노랑, 128은 무채색/중립)
- **장점:**
    - 인간의 눈이 빛과 색을 받아들이는 비선형적 지각 특성을 수학적으로 반영한 지각적 균일성(Perceptual Uniformity)을 가짐
    - 밝기(L)와 색상(a, b)이 완전히 독립되어 있어, 밝기 채널만 보정하더라도 색상이 튀거나 변질되는 현상(Color Shift) 없이 매우 자연스러운 영상 개선이 가능함
### `cv2.imread(path, option)`

- **path**: 이미지 파일 경로 (상대 경로 또는 절대 경로)
    
- **option**: 이미지를 읽어올 방식 플래그
    
    - `IMREAD_COLOR`: 3채널 BGR 컬러 이미지로 로딩 (기본값, 알파 채널 무시)
        
    - `IMREAD_GRAYSCALE`: 1채널 흑백(Grayscale) 이미지로 변환하여 로딩
        
    - `IMREAD_UNCHANGED`: 투명도(알파 채널)를 포함한 원본 채널 그대로 로딩 (예: 4채널 PNG)
        
    - `IMREAD_ANYCOLOR`: 이미지 파일이 원래 가지고 있는 색상 포맷(최대 3채널)대로 로딩
        
    - `IMREAD_ANYDEPTH`: 정밀도가 높은 이미지(16/32비트)를 8비트로 변환하지 않고 원본 비트 심도로 로딩
        
    - `IMREAD_IGNORE_ORIENTATION`: EXIF 회전 메타데이터(카메라 회전 정보)를 무시하고 파일에 저장된 원본 픽셀 배열을 그대로 로딩
        
    - `IMREAD_REDUCED_COLOR_2` / `4` / `8`: 컬러로 읽되, 해상도를 각각 **1/2, 1/4, 1/8** (가로$\cdot$세로 비율)로 축소하여 로딩
        
    - `IMREAD_REDUCED_GRAYSCALE_2` / `4` / `8`: 흑백으로 읽되, 해상도를 각각 **1/2, 1/4, 1/8**로 축소하여 로딩

### 1. `cv2.imwrite(filename, img, params)`

- **filename**: 저장할 파일 경로 및 파일명 (확장자에 따라 포맷 결정)
    
- **img**: 저장할 이미지 배열 (기본적으로 **BGR 포맷** 기준)
    
- **params** _(선택)_: 파일 저장 시 적용할 세부 옵션 (`[플래그, 값]` 쌍으로 전달)
    
    - `IMWRITE_JPEG_QUALITY`: JPEG 화질 설정 (0~100, 기본값 95)
        
    - `IMWRITE_PNG_COMPRESSION`: PNG 압축 수준 설정 (0~9, 기본값 3, 숫자가 클수록 용량 감소 및 저장 속도 감소)
        
    - `IMWRITE_WEBP_QUALITY`: WebP 화질 설정 (1~100)
        

### 2. `cv2.cvtColor(src, code)`

- **src**: 색상 공간을 변환할 원본 이미지 배열
    
- **code**: 색상 변환 알고리즘 플래그
    
    - `COLOR_BGR2RGB` / `COLOR_RGB2BGR`: BGR과 RGB 순서 상호 변환 (Matplotlib 시각화 전후 처리)
        
    - `COLOR_BGR2GRAY` / `COLOR_GRAY2BGR`: BGR 컬러 이미지를 1채널 흑백으로 변환 (또는 반대)
        
    - `COLOR_BGR2HSV` / `COLOR_RGB2HSV`: BGR/RGB를 HSV 색상 공간으로 변환 (Hue 범위: 0~179)
        
    - `COLOR_HSV2BGR` / `COLOR_HSV2RGB`: HSV 공간을 다시 BGR/RGB로 복원
        
    - `COLOR_BGR2HSV_FULL`: Hue 범위를 8비트 전체 범위인 0~255로 매핑하여 HSV 변환
        

### 3. `cv2.split(m)`

- **m**: 채널을 분리할 다채널 이미지 배열 (예: 3채널 BGR 또는 HSV 이미지)
    
- **반환값**: 각 채널별 1채널(Grayscale) 이미지 배열들의 튜플
    
    - BGR 이미지 입력 시: `b, g, r = cv2.split(img)` 형태로 분리
        
    - HSV 이미지 입력 시: `h, s, v = cv2.split(img_hsv)` 형태로 분리
        

### 4. `cv2.merge(mv)`

- **mv**: 하나의 다채널 이미지로 합칠 1채널 이미지 배열들의 튜플 또는 리스트
    
- **반환값**: 병합된 다채널 이미지 배열
    
    - BGR 병합 예시: `img_bgr = cv2.merge((b, g, r))`
        
    - HSV 병합 예시: `img_hsv = cv2.merge((h_new, s, v))`