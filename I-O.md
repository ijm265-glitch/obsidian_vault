![[Pasted image 20260912204317.png]]
좌측 : Run Mode
우측 : Config Mode
![[Pasted image 20260912205019.png]]
I/O에서 Scan을 통해 장비 찾기

![[Pasted image 20260912205156.png]]

EK1200 : CX PC 전용 버스 인터페이스 (임베디드 PC (백호프 CX)에서 신호를 받아 I/O모듈로 신호 전달)
EL3062 : 2채널 아날로그 입력 0~10V
EL4002 : 2채널 아날로그 출력 0~10V 12 bit
EL1862 : 16채널 24V 입력 3ms
EL2872 : 16채널 24V 출력 0.5A
EK1110 : 끝에 위치하여 RJ45포트 1개를 가지고 신호를 다른 장치로 보낼 때 사용 
EK1100 : PC의 신호를 받고 다른 장치로 보낼 수 있도록 RJ45포트를 2개 가짐 
PC로 제어하게 되면 EK1100 - EL3062 - EL4002 - EL1862 - EL2872 식으로 구성

![[Pasted image 20260912210556.png]]

Online Write 가능