
![[Pasted image 20261006113206.png]]

### 1. Main circuit off warning (AL.E9) (`*COP5`): 주회로 전원 OFF 경고 검출 조건

- **설정값**: `Detect in ready ON command and servo-on command`
    
- **기능**: 서보 온 지령(`Servo-on`)이나 레디 온(`Ready ON`) 지령이 들어왔는데, 정작 모터 구동용 메인 전원(L1, L3 주회로)이 안 켜져 있으면 경고 `[AL. E9 Main circuit off warning]`을 띄우는 조건입니다.
    
- **실무 기준**: 제어 전원만 켜진 상태에서 서보 온을 걸어 모터가 안 도는 실수를 즉각 감지할 수 있도록 기본값을 유지합니다.
    

### 2. Alarm history clear (`*BPS`): 알람 이력 삭제

- **설정값**: `Disabled` (기본 상태 유지)
    
- **기능**: 드라이브 내부에 누적 저장된 과거 알람 발생 기록(블랙박스 이력)을 지울 때 사용합니다.
    
- **사용법**: 평소에는 `Disabled`로 두고, 장비 시운전이 끝나고 양산으로 넘길 때 이력을 한 번 비우고 싶다면 드롭다운에서 `Clear`를 선택한 뒤 쓰기(`Write`)를 실행합니다.
    

### 3. Undervoltage alarm detection method (`*COP7`): 저전압(AL.10) 검출 방식

- **설정값**: `When undervoltage (AL.10) not occurred`
    
- **기능**: 입력 교류 전압이 떨어졌을 때 저전압 알람(`AL. 10 Undervoltage`)을 판정하는 방식을 지정합니다. 기본값 그대로 유지합니다.
    

### 4. Other axis error warning target alarm (`*FOP2`): 타축 에러 경고 대상 (다축 일체형용)

- **설정값**: `Only main circuit error (AL.24) and overcurrent (AL.32)`
    
- **설명**: 앰프 하나로 2축이나 3축을 동시에 제어하는 **`MR-J4W` 다축 일체형 앰프** 전용 항목입니다 (화면 하단 설명: _Only when it is J4W, it will be valid_).
    
- **현재 상태**: 단축형 드라이브(`MR-J4-B`)이므로 무시하고 기본값을 유지하시면 됩니다.
    

### 5. Overspeed alarm detection level (`OSL`): 과속도 알람 검출 레벨

- **설정값**: `0 r/min` (기본값)
    
- **기능**: 모터의 최대 허용 속도보다 더 낮은 특정 속도에서 과속도 알람(`AL. 50 Overspeed`)을 조기에 터뜨리고 싶을 때 상한 rpm을 지정하는 기능입니다.
    
- **의미**: `0`으로 두면 모터 자체의 최대 회전 속도(보통 6000 rpm 등)를 기준으로 정상 감시하므로 변경할 필요가 없습니다.