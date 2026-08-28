### Function
- 내부 상태를 저장하지 않는 일회성 계산용 명령 블록입니다. 동일한 입력값이 들어오면 실행 시점과 관계없이 항상 동일한 출력값을 반환합니다.



### Function Block
- 자신만의 전용 메모리를 보유하여 이전 상태나 연산 결과를 기억하는 부품 형태의 명령 블록입니다. 동일한 입력값이 들어오더라도 내부 상태에 따라 다른 출력값을 반환할 수 있습니다.


|**구분**|**Function (FUN)**|**Function Block (FB)**|
|---|---|---|
|**상태 유지 (State)**|**Stateless (기억 불가)**<br><br>  <br><br>실행 완료 후 내부 변수 초기화|**Stateful (기억 가능)**<br><br>  <br><br>다음 스캔 타임까지 내부 상태 유지|
|**인스턴스 (Instance)**|**불필요**<br><br>  <br><br>별도 메모리 할당 없이 기능만 호출|**필요**<br><br>  <br><br>사용 시마다 고유한 메모리 이름(인스턴스) 할당|
|**객체지향 비유**|Pure Function / Static Method|Class 및 Class Instance|
|**대표적인 예시**|사칙연산(`ADD`, `SUB`), 비교(`EQ`), `INC` 등|타이머(`TON`, `TOF`), 카운터(`CTU`, `CTD`), PID 등|

### Example
![[Pasted image 20260820114137.png]]
*Function*인 INC
별도의 변수를 사용하지 않고도 INC Funtion을 가져와 실행할 수 있다.
![[Pasted image 20260820114246.png]]
*Function Block인* CTD_INT
INST1이라는 인스턴스를 만들어서 그 인스턴스마다 고유의 CV (Current Value)를 가지도록 함
INST2라는 인스턴스를 만들어서 그 인스턴스의 CV를 출력해도 INST1의 CV와는 다르다는 것을 알 수있음