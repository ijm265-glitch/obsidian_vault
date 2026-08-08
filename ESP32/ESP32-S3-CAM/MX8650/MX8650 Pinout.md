![[Pasted image 20260808164838.png]]

| **Pin No.** | **Symbol** | **I/O** | **Function**                                                                                      |
| ----------- | ---------- | ------- | ------------------------------------------------------------------------------------------------- |
| **1**       | NC         | -       | No function                                                                                       |
| **2**       | MOTSWK     | O       | Motion detect                                                                                     |
| **3**       | SDIO       | I/O     | Serial interface data                                                                             |
| **4**       | SCLK       | I       | Serial interface clock                                                                            |
| **5**       | LED_CNTL   | O       | LED control                                                                                       |
| **6**       | GND        | P       | GND                                                                                               |
| **7**       | VDD        | P       | Power supply (2.0V~3.5V) for internal power regulator, VDDA (1.8V) is the power regulator output. |
| **8**       | VDDA       | P       | Analog/Digital supply voltage(1.8V)<br>Power supply (1.73V~1.87V) for low power operation voltage |
### 마우스 고장 원인 추론
- 다이소에서 구매한 버티컬 마우스가 고장이 남
- 증상은 마우스 전원을 켰을 때 LED가 잠깐 켜졌다가 꺼지면서 마우스가 동작하지 않는것
- 고장 원인이 MX8650A칩의 고장에 있다고 판단한다
	1. 마우스 전원이 켜지면 MX86550A 칩이 초기화 되면서 바닥 표면을 확인하기 위해 LED_CNTL에 신호를 보냄
	2. 광학 카메라가 바닥 패턴의 변화를 확인
	3. 카메라의 고장으로 바닥 패턴의 변화를 감지하지 못하여 전력을 아끼기 위해 LED_CNTL을 LOW로 유지

- 원래 절전상태에서 LMB또는 RMB로 LED가 활성화 되었는데 그것도 안되는 이유는?
	- 모르겠다 
	- yc1011-s 블루투스 모듈에도 문제가 있는가?

고장을 확인하기 위해서 esp32와 motswk, sdio, sclk, vdd, gnd를 연결하여 확인해봄

모션을 입력하여도  motswk에 신호가 없는것을 확인함. 