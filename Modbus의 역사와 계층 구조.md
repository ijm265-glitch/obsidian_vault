Modbus RTU, ASCII, TCP 전부 동일한 PDU(Protocol Data Unit)을 가지고 Framing과 물리 계층이 바뀜
$$\text{PDU} = \text{Function Code (1 Byte)} + \text{Data (N Bytes)}$$

| **계층 구분**            | **Modbus RTU**             | **Modbus ASCII**                 | **Modbus TCP**                 |
| -------------------- | -------------------------- | -------------------------------- | ------------------------------ |
| **응용 계층 (L7)**       | **공통 Modbus PDU**          | **공통 Modbus PDU**                | **공통 Modbus PDU**              |
| **프레임 캡슐화**          | Slave ID + CRC16           | Header(`:`) + LRC + Tail(`\r\n`) | **MBAP Header** (7바이트)         |
| **표현 방식**            | 순수 바이너리 (Hex)              | ASCII 문자열 변환                     | 순수 바이너리 (Hex)                  |
| **전송/네트워크 (L4/L3)**  | 없음 (시리얼 스트림)               | 없음 (시리얼 스트림)                     | **TCP/IP** (기본 포트 **502**)     |
| **물리/데이터링크 (L1/L2)** | **RS-485**, RS-422, RS-232 | **RS-485**, RS-232               | **Ethernet** (Cat.5e/6, Wi-Fi) |

Function Code란 Client가 Server에게 무슨 작업을 할지 명령하는 1바이트 (0x01~0xFF)의 지시어
