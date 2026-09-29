#PLC_Client 

Modbus RTU 사용자 프레임 실습

![[Pasted image 20260929115655.png]]
P2P 드라이버를 사용자 프레임 정의로 사용

사용자 프레임 정의 우클릭 - 그룹 생성 - 송신 그룹, 수신 그룹 1개씩 생성
송신그룹 우클릭 -> 프레임 추가 -> (Head, Body, Tail)

### 1. Modbus RTU 기본 프레임 구조 (총 최대 256바이트)

### 송신

| **구분** | **프레임 시작/구분**  | **슬레이브 주소** | **기능 코드 (Function Code)** | **데이터 필드 (Data)**  | **오류 검출 (CRC-16)**               | **프레임 종료/구분**  |
| ------ | -------------- | ----------- | ------------------------- | ------------------ | -------------------------------- | -------------- |
| **길이** | $\ge 3.5$ Char | 1 Byte      | 1 Byte                    | $0 \sim 252$ Bytes | 2 Bytes                          | $\ge 3.5$ Char |
| **순서** | 무음 대기          | 1번째 바이트     | 2번째 바이트                   | $N$개 바이트           | Low Byte $\rightarrow$ High Byte | 무음 대기          |
**Head** : 없음
**Body** : Device_id + Function Code + Address + Data
**Tail** : CRC-16
### 수신

|**구분**|**프레임 시작/구분**|**슬레이브 주소**|**기능 코드 (Function Code)**|**데이터 필드 (Data)**|**오류 검출 (CRC-16)**|**프레임 종료/구분**|
|---|---|---|---|---|---|---|
|**길이**|$\ge 3.5$ Char|1 Byte|1 Byte|$1 \sim 251$ Bytes|2 Bytes|$\ge 3.5$ Char|
|**순서**|무음 대기|1번째 바이트|2번째 바이트|$N$개 바이트|Low Byte $\rightarrow$ High Byte|무음 대기|
**Head :** 없음
**Body :** `Device_id + Function Code + Byte Count + Data`
_(쓰기 응답인 FC 06/10의 경우: `Device_id + Function Code + Address + Value/Quantity`)_
**Tail :** CRC-16