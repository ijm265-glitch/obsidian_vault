공장 자동화 및 계측 분야의 사실상 국제 표준(De facto standard)인 **Modbus 프로토콜**의 통신 방식별 비교와 **Master-Slave 구조 및 Register Map의 핵심 개념**입니다.

## 1. Modbus RTU vs Modbus TCP 비교

Modbus는 L7(응용 계층) 프로토콜로, 하부의 물리/네트워크 계층에 따라 RTU(시리얼 기반)와 TCP(이더넷 기반)로 나뉩니다.

|**구분**|**Modbus RTU**|**Modbus TCP**|
|---|---|---|
|**하위 계층 (L1~L4)**|**RS-485 / RS-232** (시리얼)|**Ethernet (IP / TCP Port 502)**|
|**통신 방식**|반두위 (Half-Duplex, 2선식 기준)|전두위 (Full-Duplex)|
|**전송 데이터 포맷**|**이진 데이터 (Binary)**|**이진 데이터 + MBAP 헤더**|
|**에러 검출 (L2/L3)**|**CRC-16** (패킷 끝에 2Byte 부착)|TCP/IP 및 Ethernet의 CRC가 자체 담당|
|**속도 및 거리**|느림 ($9.6k \sim 115.2\text{kbps}$), 최대 $1.2\text{km}$|**매우 빠름** ($100\text{Mbps} \sim 1\text{Gbps}$), 거리 제한 없음 (네트워크 망)|
|**네트워크 구조**|Master 1대 : Slave 최대 247대 (Multi-drop)|Client(Master) 여러 대 : Server(Slave) 여러 대|

```
[ Modbus RTU 프레임 ]
┌───────────┬───────────┬──────────────────┬───────────┐
│ Slave ID  │ Function  │       Data       │  CRC-16   │  (RS-485 라인으로 전송)
│  (1 Byte) │  (1 Byte) │  (N Bytes...)    │  (2 Bytes)│
└───────────┴───────────┴──────────────────┴───────────┘

[ Modbus TCP 프레임 ]
┌────────────────────────┬───────────┬──────────────────┐
│   MBAP Header (7 Bytes)│ Function  │       Data       │  (TCP/IP 포트 502로 전송)
│ (Transaction ID, Unit ID)│  (1 Byte) │  (N Bytes...)    │
└────────────────────────┴───────────┴──────────────────┘
```

> **💡 핵심 차이:** Modbus TCP는 RTU 프레임에서 **Slave ID를 Unit ID로 바꾸고, CRC를 제거한 뒤, 앞에 7Byte의 MBAP(Modbus Application Protocol) 헤더를 붙여 TCP/IP 패킷에 감싸 보낸 형태**입니다.

## 2. Master-Slave (Client-Server) 구조

Modbus RTU는 철저한 **폴링(Polling) 기반의 1:N 토폴로지**를 따릅니다.


```
[ Master (PLC / HMI / PC) ] 
       │  ▲
       │  │ (1. Request: "2번 Slave, 40001번 레지스터 값 내놔")
       │  │ (2. Response: "내 40001번 값은 25.4℃야")
       ▼  │
  ┌────┴──┴─────────────────┬─────────────────────────┐
  ▼                         ▼                         ▼
[ Slave 1번 ]          [ Slave 2번 ]             [ Slave 3번 ]
```

1. **Master (주도자):**
    
    - 네트워크에서 **오직 Master만 먼저 말을 걸(Request) 수 있습니다.**
        
    - Slave들에게 순차적으로 데이터를 달라고 물어보거나(Polling), 명령을 내립니다.
        
2. **Slave (응답자):**
    
    - 평소에는 마냥 듣고만 있다가, **자신의 Slave ID가 적힌 패킷이 들어왔을 때만 답변(Response) 패킷을 송신**합니다.
        
    - Slave끼리는 절대로 서로 직접 대화할 수 없습니다.
        

_(참고: Modbus TCP에서는 Master를 **Client**, Slave를 **Server**라는 용어로 부릅니다.)_

## 3. Register Map (레지스터 맵) 개념

Modbus 장비(센서, 인버터, 전력계 등) 내부의 메모리는 데이터의 타입과 읽기/쓰기 권한에 따라 4가지 영역(Table)으로 엄격하게 격리되어 관리됩니다. 이 메모리 주소 배치도를 **Register Map**이라고 부릅니다.

### 1) Modbus 4대 메모리 영역

|**영역 구분**|**데이터 타입**|**권한 (Access)**|**주소 범위 (표준)**|**주요 용도**|
|---|---|---|---|---|
|**Discrete Inputs**|1 bit (On/Off)|Read-Only|`10001 ~ 19999`|디지털 입력 (스위치, 센서 감지 여부)|
|**Coils (Discrete Outputs)**|1 bit (On/Off)|**Read / Write**|`00001 ~ 09999`|디지털 출력 (솔레노이드 밸브, 릴레이, 램프 ON/OFF)|
|**Input Registers**|16 bit (Word)|Read-Only|`30001 ~ 39999`|아날로그 입력 (현재 온도, 압력, 유량 측정값)|
|**Holding Registers**|16 bit (Word)|**Read / Write**|`40001 ~ 49999`|아날로그 설정값 (목표 온도, 모터 속도 SV, 파라미터)|

### 2) 주요 Function Code (기능 코드)

Master가 Slave에게 "어떤 메모리 영역을 읽거나 쓸 것인가"를 지시할 때 사용하는 1바이트 명령어입니다.

- **`01 (0x01)` Read Coils:** 코일(디지털 출력) 상태 읽기
    
- **`02 (0x02)` Read Discrete Inputs:** 디지털 입력 상태 읽기
    
- **`03 (0x03)` Read Holding Registers:** 홀딩 레지스터(아날로그 설정값/데이터) 읽기 _(가장 많이 사용)_
    
- **`04 (0x04)` Read Input Registers:** 인풋 레지스터(아날로그 측정값) 읽기
    
- **`05 (0x05)` Write Single Coil:** 단일 코일 ON/OFF 제어
    
- **`06 (0x06)` Write Single Register:** 단일 홀딩 레지스터 값 변경
    
- **`16 (0x10)` Write Multiple Registers:** 여러 홀딩 레지스터 값 연속 변경
    

### 💡 데이터 읽기 예시 (온도 센서)

Master가 2번 Slave 센서의 현재 온도(`30001`번 주소)를 읽고 싶을 때:

1. **Master $\rightarrow$ Slave 2 전송 (Modbus RTU):**
    
    - `[0x02]` (Slave ID) + `[0x04]` (Read Input Reg) + `[0x00 0x00]` (Start Address 0) + `[0x00 0x01]` (Read 1 Reg) + `[CRC]`
        
2. **Slave 2 $\rightarrow$ Master 응답:**
    
    - `[0x02]` (Slave ID) + `[0x04]` (Function Code) + `[0x02]` (Byte Count) + `[0x00 0xFA]` ($250 \rightarrow 25.0^\circ\text{C}$) + `[CRC]`