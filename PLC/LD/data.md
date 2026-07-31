```
S: 전송하고자 하는 데이터 또는 데이터가 들어있는 디바이스 번호
D: 전송된 데이터를 저장할 디바이스 번호
```

**MOV (MOVE)**
- 구문 : MOV S D

- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호

- 동작 방식 : S로 지정된 디바이스의 워드 데이터를 D로 전송

**GMOV (GROUP MOVE)**
- 구문 : GMOV S D N

- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호

- 동작 방식 : S로 부터 N개의 오름차순 디바이스에 저장된 데이터를 D부터 N개의 오름차순 디바이스에 전송

- 예시 : GMOV D0 D3 3
	- D0, D1, D2에 저장된 값을 각각 D3, D4, D5에 전송

**BMOV (BIT MOVE)**
- 구문 : BMOV S D Z

- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호

- 동작 방식 : S에 저장되어 있는 데이터의 하위 비트 Z개를 D로 전송

- 예시 : BMOV D0 D3 7
	- D0: 0000 0000 1011 0100*이고 하면 이 함수의 결과는 
	- D3: 0000 0000 0**011 0100**

**FMOV (FILE MOVE)**
- 구문 : FMOV S D N

- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호

- 동작 방식 : S에 저장되어 있는 데이터를 D부터 N개의 오름차순 디바이스에 전송

- 예시 : BMOV D0 D3 7
	- D0: 0000 0000 1011 0100*이고 하면 이 함수의 결과는 
	- D3: 0000 0000 0**011 0100**

**INC (INCREMENT)
DEC (DECREMENT)**
- 구문 : INCA D

- 입력 조건 : HIGH

- 동작 방식 : D에 저장되어 있는 데이터를 1씩 증가시키거나 감소시킨다

**ADD 
SUB (SUBTRACTION)
MUL (MULTIFLY)
DIV (DIVIDE)**
- 구문 : ADD S1 S2 D

- 입력 조건 : LOW -> HIGH 

- 동작 방식 : S1과 S2의 데이터를 기본연산한 결과를 D에 전송한다

**SCAL (SCALE)** 
- 구문 : ADD S1 S2 S3 D

- 입력 조건 : LOW -> HIGH 

- 동작 방식 : 특정 범위 $T_0\sim T_1$에 속하는 데이터가 S1에 저장되어 있을 때 