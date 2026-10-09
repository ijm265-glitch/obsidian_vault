![[Pasted image 20261006114531.png]]

### 1. Machine resonance suppression filter 1 (`FILT, NH1, NHQ1`): 1차 노치 필터

- **기능**: 서보 모터를 기구물(볼스크루, 벨트 등)에 연결해 게인을 올리다 보면 특정 고유 주파수에서 "삐-" 하는 고주파 공진이 발생합니다. 이때 해당 공진 주파수 대역의 토크 지령을 쏙 파내어(Notch) 진동을 죽이는 대역 저지 필터(Band-stop filter)입니다.
    
- **`Filter tuning mode selection` (`Disabled`)**:
    
    - `Disabled`: 노치 필터 기능을 끕니다.
        
    - `Auto`: 서보 드라이브가 회전 중 공진 주파수를 실시간으로 스스로 찾아내어 자동으로 주파수를 맞춰 깎아냅니다.
        
- **`Frequency` (`4500 Hz`)**:
    
    - 깎아낼 목표 공진 주파수(10 ~ 4500Hz)입니다. 기본값 4500Hz는 가청 공진 대역 바깥의 최대치로 설정되어 있어 실질적으로 동작하지 않는 대기 상태입니다.
        
- **`Notch depth` / `Notch width`**:
    
    - 해당 주파수를 얼마나 깊게(-40dB 등), 얼마나 넓은 폭($\alpha=2$)으로 파낼 것인지 결정하는 필터 감쇠 형태입니다.
        

### 2. Machine resonance suppression filter 2 (`NHQ2, NH2`): 2차 노치 필터

- **기능**: 기계 구조상 공진점이 2개 이상일 때 두 번째 공진 주파수를 추가로 억제하기 위한 보조 노치 필터입니다.
    
- **현재 상태**: 1차 필터와 마찬가지로 기본값 **`Disabled`** 상태입니다.