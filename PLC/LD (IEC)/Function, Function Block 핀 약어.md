EN (Enable) : 실행 허용 입력 (BOOL)
ENO (Enable Output) : 실행 완료 출력 (BOOL)
IN (Input) : 데이터 입력
- 함수를 호출할 때 전달하는 값
OUT (Output) : 데이터 출력
- 함수를 호출한 후 값을 반환받을 변수
### Counter 관련
**공통**
CV (Current Value) : Counter의 현재값 
PV (Preset Value) : Counter의 설정값

**CTU**
CU (Count Up) : 신호의 rising을 검출하여 CV를 1 증가시킨다
R (Reset) : 신호의 rising을 검출하여 CV를 0으로 만든다

**CTD**
CD (Count Down) : 신호의 rising을 검출하여 CV를 1 감소시킨다.
LD (Load) : 신호의 rising을 검출하여 CV를 PV로 만든다 

### Timer 관련
IN : 타이머가 작동하기 위한 조건
Q : 타이머의 출력
ET (Elapsed Time) : 타이머가 작동하고 경과한 시간
PT (Preset Time) : 타이머에 설정한 시간

