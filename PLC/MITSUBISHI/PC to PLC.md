Ethernet Cable을 사용하여 [[SLMP(MC Protocol)]]를 사용한다.
PC Client
PLC Server로 동작
Q PLC의 경우 Modbus TCP를 지원하지 않으므로 SLMP를 이용하여야 함 - Modbus TCP를 사용하기 위해 별도 모듈을 이용할 수 있으나 불필요함



PLC Parameter - Built-in Ethernet Port Setting
![[Pasted image 20260930102304.png]]

[[2.6 ARP 프로토콜]]
**IP Address** : PLC의 IP Address를 설정한다. Q PLC의 Default IP는 `192.168.3.39`이다

**Subnet Mask Pattern** : IP Address의 어디까지가 Subnet인지를 가려내기 위해 작성. 

**Default Router IP Address** : 다른 네트워크 대역 (서브넷)끼리 통신하는 경우 라우터(게이트웨이)의 Address를 작성한다. `ex). 192.168.3.9와 192.168.2.4가 통신하는 경우 서브넷이 다르기 때문에 라우터를 거쳐야 함`