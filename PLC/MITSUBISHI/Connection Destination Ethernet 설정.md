PC와 PLC를 연결하기 위해 Connection Destination에서 일반적으로 usb를 사용하지만 
Ethernet Cable을 사용한다고 할 때 다음의 절차를 따를 수 있다.
### 1. PC 랜카드의 IP 주소 수정
- 일반적으로 PC 랜카드는 [[DHCP IP 할당 (고정 Private IP 설정)|DHCP]]모드이기 때문에 공유기가 할당하는 IP를 사용하게 됨 
- PC to PLC의 경우 공유기가 없으므로 IP가 임의로 부여되기 때문에 PLC 기본 IP Address인 `192.168.3.39`와 다른 서브넷을 부여받게될 확률이 높음
- 따라서 다음의 절차로 PC의 IP Address를 사용자가 설정하여 같은 서브넷을 가진 IP Address를 사용하게 함

Windows
Control Panel - Network and Internet -  Network and Sharing Center - Change adapter settings - Ethernet - Properties - IPv4 - Use the following IP address에서 PLC와 같은 서브넷을 사용하도록 설정한다.

![[Pasted image 20260930111754.png]]

PLC의 Built-in Ethernet Port와 연결 후
Gx Works2 - Connection Destination - PLC Direct Coupled Setting -  Ethernet - Adapter : PC LAN카드 - Connection Test

