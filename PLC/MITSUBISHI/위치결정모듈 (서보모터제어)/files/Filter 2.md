![[Pasted image 20261006114551.png]]

### 1. Machine resonance suppression filter 3 & 4 (`NHQ3, NH3 / NHQ4, NH4`): 3·4차 노치 필터

- **기능**: 앞서 Filter 1 화면에 있던 1·2차 노치 필터 외에, 기구물이 복잡하여 공진점이 3개, 4개씩 생길 때 추가로 주파수를 파내기 위한 확장 노치 필터입니다.
    
- **현재 설정값**: 모두 기본값인 **`Disabled`** 상태입니다.
    
- **기준**: 현재는 부하가 연결되지 않은 단독 모터 테스트이므로 기계 공진이 없어 기본값(`Disabled`)을 유지합니다.
    

### 2. Low-pass filter (`VFBF, LPF` / `PB23`): 1차 저역 통과 필터 (토크 지령 LPF)

- **기능**: 토크 지령에 섞여 들어오는 고주파 전기 노이즈나 급격한 토크 튀는 성분을 깎아내어 제어를 부드럽게 만들어주는 필터입니다.
    
- **설정값**: **`Automatic setting`** (`3141 rad/s`)
    
- **원리**: `Auto tuning mode 1`과 연동되어 속도 루프 게인에 맞춰 드라이브가 최적의 차단 주파수(기본 약 500Hz 상당인 $3141\text{ rad/s}$)를 자동으로 계산해 걸어줍니다. 손댈 필요 없이 `Automatic setting`을 유지합니다.
    

### 3. Shaft resonance suppression filter (`VFBF, NHF` / `PB23`): 축 공진 억제 필터

- **기능**: 모터 축과 커플링, 감속기 체결부 사이의 비틀림(Torsion) 강성 때문에 발생하는 축 공진 진동을 억제하는 전용 필터입니다.
    
- **설정값**: **`Automatic setting`**
    
- **원리**: 이 항목 역시 드라이브의 오토 튜닝 알고리즘이 내부 상태를 감시하며 필요시 자동으로 적응 제어하므로 **`Automatic setting`** 그대로 두시면 됩니다.