![[Pasted image 20261006113236.png]]

### 1. Vibration tough drive (`*TDS, OSCL1, *OSCL2`): 진동 내구 운전 기능

기구물 노후화나 볼트 풀림 등으로 공진 주파수가 틀어져 모터가 "우웅-" 하고 발진(떨림)할 때 스스로 진동을 억제하는 기능입니다.

- **`Vibration tough drive selection` (`Disabled`)**:
    
    - 활성화(`Enabled`)하면 모터가 진동을 감지했을 때 필터(머신 공진 억제 필터)를 자동으로 재조정하여 진동을 가라앉히고 운전을 지속합니다.
        
    - 일반 환경에서는 비정상 떨림을 초기에 잡아 점검해야 하므로 기본값인 `Disabled`를 유지합니다.
        
- **`Oscillation detection level` (`50 %`)**:
    
    - 발진(진동)으로 판단할 토크 리플 진폭의 감도 기준입니다 (기본 50%).
        
- **`Oscillation detection alarm selection` (`Turn on [AL.54 Oscillation Detection Error]`)**:
    
    - 진동이 지속될 경우 발진 검출 에러(`AL. 54`)를 띄우고 안전하게 멈추도록 지정합니다.
        

### 2. SEMI-F47 function (`*TDS, CVAT, *AOP5`): 순간 정전 대응 규격

반도체/FPD 생산 라인 표준 규격인 SEMI-F47(순간 전압 강하 내성 규격)을 만족하기 위한 전원 보상 기능입니다.

- **`SEMI-F47 function selection` (`Disabled`)**:
    
    - 공장 전원에 수십~수백 ms 수준의 찰나의 순간 정전(순저)이 발생했을 때 바로 `AL. 10`(저전압)을 띄우며 라인을 세우지 않고, 제어 전원을 유지하며 버티는 모드입니다.
        
    - 일반 산업 현장이나 단독 모터 테스트 환경에서는 필요하지 않으므로 기본값 `Disabled`를 유지합니다.
        
- **`Instantaneous power failure detection time` (`200 ms`)**:
    
    - 순간 정전 발생 시 몇 ms까지 저전압 알람을 유예하고 버틸 것인지 설정하는 시간입니다.