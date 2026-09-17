  ![[Pasted image 20260917115422.png]]## XG5000 - PLC
![[Pasted image 20260917112112.png]]
해당 란은 XG5000과 PLC가 어떻게 통신할지를 결정하는 곳으로![[Pasted image 20260917112152.png]]
옵션을 선택할 수 있다. 
USB가 가장 간편하므로 USB를 사용한다
## PLC에 Write하는 네트워크 옵션
![[Pasted image 20260917112413.png]]
XGC-DR32H는 2개의 통신 채널이 있다.
RS232C와 RS485가 있으며 RS232C는 HMI와의 통신을 위해
RS485를 PC (C#) 으로 통신하기 위해 사용하겠다
국번 0은 Master이기 때문에 1로 바꾸어 주고
동작모드는 MODBUS RTU (RS232, 422, 485) 프로토콜로 설정한다.
이 때 PLC의 디바이스 정보를 PC에서 요청하는 것이기 때문에 서버라는 말이 붙는다
![[Pasted image 20260917113118.png]]
이 때 MODBUS 설정이 매우 중요한데
PC 클라이언트가 PLC 서버에 Read, Write 요청을 할 때 offset으로 요청을 하기 때문에 
PLC 서버는 이 설정값에 대해 offset된 device value를 return한다.
기본값은 P device이기 때문에 특수한 경우가 아니라면 비트 디바이스는 M, 워드 디바이스는 D의 0번 device를 지정해주도록 한다
## PLC에 쓰기
최종적으로 네트워크 옵션 설정을 마쳤다면 XG5000과 PLC를 접속하여 쓰기 작업을 수행한다
이 때 네트워크 옵션이 변경되었다면 PLC를 Reset하여서 네트워크 옵션이 반영되도록 해야한다
PLC Reset은 XG5000 - Online - Reset/Clear로 접근하여 수행한다.


