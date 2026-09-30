Modbus RTU, ASCII, TCP 전부 동일한 PDU(Protocol Data Unit)을 가지고 Framing과 물리 계층이 바뀜
$$\text{PDU} = \text{Function Code (1 Byte)} + \text{Data (N Bytes)}$$

| **계층 구분**            | **Modbus RTU**             | **Modbus ASCII**                 | **Modbus TCP**                 |
| -------------------- | -------------------------- | -------------------------------- | ------------------------------ |
| **응용 계층 (L7)**       | **공통 Modbus PDU**          | **공통 Modbus PDU**                | **공통 Modbus PDU**              |
| **프레임 캡슐화**          | Slave ID + CRC16           | Header(`:`) + LRC + Tail(`\r\n`) | **MBAP Header** (7바이트)         |
| **표현 방식**            | 순수 바이너리 (Hex)              | ASCII 문자열 변환                     | 순수 바이너리 (Hex)                  |
| **전송/네트워크 (L4/L3)**  | 없음 (시리얼 스트림)               | 없음 (시리얼 스트림)                     | **TCP/IP** (기본 포트 **502**)     |
| **물리/데이터링크 (L1/L2)** | **RS-485**, RS-422, RS-232 | **RS-485**, RS-232               | **Ethernet** (Cat.5e/6, Wi-Fi) |

### Function Code
Function Code란 Client가 Server에게 무슨 작업을 할지 명령하는 1바이트 (0x01~0xFF)의 지시어

| **기능 코드 (Hex)** | **이름 (기능)**              | **조작 대상**            | **읽기/쓰기** | **실무 사용 예시**                     |
| --------------- | ------------------------ | -------------------- | --------- | -------------------------------- |
| **`0x01`**      | Read Coils               | 1 Bit                | **읽기**    | 릴레이/솔레노이드 밸브 ON/OFF 상태 확인        |
| **`0x02`**      | Read Discrete Inputs     | 1 Bit                | **읽기**    | 리미트 스위치, 포토 센서 접점 감지 상태 확인       |
| **`0x03`**      | Read Holding Registers   | 16 Bit               | **읽기**    | 설정된 파라미터 값(목표 온도, 모터 속도 설정치) 읽기  |
| **`0x04`**      | Read Input Registers     | 16 Bit               | **읽기**    | **현재 센서 측정값(온도, 습도, 전압, 전류) 읽기** |
| **`0x05`**      | Write Single Coil        | 1 Bit                | **단일 쓰기** | 릴레이 1개 강제 ON 또는 OFF              |
| **`0x06`**      | Write Single Register    | 16 Bit               | **단일 쓰기** | 설정값 1개 변경 (국번 변경, 모터 목표 RPM 변경)  |
| **`0x0F`**      | Write Multiple Coils     | 1 Bit $\times$ 여러 개  | **연속 쓰기** | 릴레이 8개를 한 번에 ON/OFF 제어           |
| **`0x10`**      | Write Multiple Registers | 16 Bit $\times$ 여러 개 | **연속 쓰기** | 레지스터 여러 개를 한 번에 연속 변경            |

```에러 발생시 원래 보냈던 Function Code의 MSB를 1로 바꾸어 회신함 (원래 코드 + 0x80)
ex). 
마스터가 0x04 요청 -> 슬레이브 응답: 0x04 (정상 실행)
마스터가 0x04 요청 -> 슬레이브 응답: 0x84 
그리고 그 뒤에 에러 원인 코드를 붙임
```