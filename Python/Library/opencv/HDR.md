
#### 1. 다중 노출 이미지 및 노출 시간 로드 (Data Preparation)

- **개념:** 노출 시간(셔터 스피드)만 다르게 찍은 여러 장의 LDR 이미지와 해당 셔터 속도 배열(`times`)을 준비합니다.
    
- **코드 매핑:**
    ```Python
    def readimageAndTimes():
        filenames = ["img_0.033.jpg", "img_0.25.jpg", "img_2.5.jpg", "img_15.jpg"]
        times = np.array([1/ 30, 0.25, 2.5, 15.0], dtype=np.float32)
        images = [cv2.imread(fn) for fn in filenames]
        return images, times
    
    images, times = readimageAndTimes()
    ```
#### 2. 정렬 (Alignment - MTB)

- **개념:** 손떨림으로 인한 미세한 위치 오차를 잡기 위해, 노출 영향을 받지 않도록 픽셀을 흑백 비트맵(0과 1)으로 바꾼 뒤 비트 연산으로 빠르게 정렬합니다.
    
- **코드 매핑:**
    ```    Python
    alignMTB = cv2.createAlignMTB()
    alignMTB.process(images, images) # images 리스트 내의 위치를 맞추어 덮어씀
    ```
    
#### 3. 카메라 응답 함수 추정 (CRF Calibration - Debevec)
[[CRF (Camera Response Function )]]
- **개념:** 입력된 이미지들과 노출 시간 정보를 바탕으로, 카메라 센서의 비선형적 톤 커브(역 카메라 응답 함수, $f^{-1}$)를 계산합니다.
    
- **코드 매핑:**
    ```Python
    calibrateDebevec = cv2.createCalibrateDebevec()
    responseDebevec = calibrateDebevec.process(images, times) # (256, 1, 3) 크기의 LUT 생성
    
    # CRF 시각화 (Squeeze로 (256, 3) 2차원 변환 후 RGB 채널 플롯)
    y = np.squeeze(responseDebevec)
    plt.plot(x, y[:, 0], "b", x, y[:, 1], "g", x, y[:, 2], "r")
    ```
#### 4. Radiance Map 합성 (HDR Merging)

- **개념:** 구해낸 CRF와 노출 시간을 이용해 픽셀값들을 실제 물리적 빛의 세기(Radiance)로 복원하고, 극단적 포화 영역(0 및 255 근처)에 가중치를 낮게 두어 가중 평균 합성합니다.
- **코드 매핑:**
    ```Python
    mergeDebevec = cv2.createMergeDebevec()
    hdrDebevec = mergeDebevec.process(images, times, responseDebevec) # 32-bit float HDR 이미지 출력
    ```
#### 5. 톤 매핑 및 디스플레이 재현 (Tone Mapping)

- **개념:** 32비트 HDR 데이터의 넓은 Dynamic Range를 일반 디스플레이(8비트, $0 \sim 255$)에서 볼 수 있도록 알고리즘별 특성에 맞춰 압축 및 색상을 재현합니다.
    
- **코드 매핑:**
    - **① Drago 방식:** 인간 시각의 대수적(Logarithmic) 반응 특성을 반영한 톤 매핑
        ```Python
        tonemapDrago = cv2.createTonemapDrago(1.0, 0.7)
        ldrDrago = tonemapDrago.process(hdrDebevec)
        ldrDrago = 3 * ldrDrago # 밝기 스케일링
        cv2.imwrite("ldr-Drago.jpg", 255 * ldrDrago)
        ```
        
    - **② Reinhard 방식:** 사진학적(Photographic) 명암 보정 기반 톤 매핑
        ```Python
        tonemapReinhard = cv2.createTonemapReinhard(1.5, 0, 0, 0)
        ldrReinhard = tonemapReinhard.process(hdrDebevec)
        cv2.imwrite("ldr-Reinhard.jpg", ldrReinhard * 255)
        ```
    - **③ Mantiuk 방식:** 명암 대비(Contrast) 표현 극대화 기반 톤 매핑
        ```Python
        tonemapMantiuk = cv2.createTonemapMantiuk(2.2, 0.85, 1.2)
        ldrMantiuk = tonemapMantiuk.process(hdrDebevec)
        ldrMantiuk = 3 * ldrMantiuk # 밝기 스케일링
        cv2.imwrite("ldr-Mantiuk.jpg", ldrMantiuk * 255)
        ```
- **출력 보정 (Matplotlib 시각화):**
    OpenCV의 BGR 채널을 Matplotlib의 RGB 순서에 맞추기 위해 슬라이싱(`[:, :, ::-1]`) 및 범위를 $0 \sim 1$로 제한(`np.clip`)하여 시각화합니다.
    ```Python
    plt.imshow(np.clip(ldrResult, 0, 1)[:, :, ::-1])
    ```