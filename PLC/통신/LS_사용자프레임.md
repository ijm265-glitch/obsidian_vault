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

#### Body
![[Pasted image 20260929122540.png]]
Device_id : 01
Function Code :  04 (Read_Input_Resisters)
Address : 0001 (Input Resister의 01오프셋 주소)
Data : 0002 (Input Resister의 01오프셋 주소로부터 2개의 주소의 값을 읽어라)

#### Tail
![[Pasted image 20260929124107.png]]
BCC추가
![[Pasted image 20260929124216.png|236]]
방식은 CRC 16

#### 수신 분석
Body의 모든 영역을 사용하므로 시작위치와 끝위치를 다음과 같이 설정
![[Pasted image 20260929124330.png]]
다음과 같이 P2P 블록을 설정하면 끝

![[Pasted image 20260929124512.png]]

프레임 모니터에서 다음과 같은 결과를 확인
송신값에 따라 수신하였고 수신 프레임을 설정하지 않아 결과는 모름임
`01 04 04 01 13 01 E1 CA 65`
device_id : `01` 
Function_code : `04` (레지스터 읽기에 대한 Return값임을 암시)
Byte Count : `04` (4바이트)
Data : `01 13`, `01 E1` (각각 10진수로 275, 481이며 온도 27.5, 습도 48.1을 의미)
CRC-16 : `CA 65`
### 수신

|**구분**|**프레임 시작/구분**|**슬레이브 주소**|**기능 코드 (Function Code)**|**데이터 필드 (Data)**|**오류 검출 (CRC-16)**|**프레임 종료/구분**|
|---|---|---|---|---|---|---|
|**길이**|$\ge 3.5$ Char|1 Byte|1 Byte|$1 \sim 251$ Bytes|2 Bytes|$\ge 3.5$ Char|
|**순서**|무음 대기|1번째 바이트|2번째 바이트|$N$개 바이트|Low Byte $\rightarrow$ High Byte|무음 대기|

**Head :** 없음
**Body :** `Device_id` + `Function Code` + `Byte Count` + `Data`
**Tail :** CRC-16

#### Body
![[Pasted image 20260929130119.png]]

#### Tail
![[Pasted image 20260929130133.png]]

![[Pasted image 20260929130202.png]]

설정에서 수신한 데이터를 D100과 D101에 저장하도록 한다.
### 프레임 모니터 접근
온라인 - 통신모듈 설정 및 진단 - 시스템 진단 - 그림 우클릭 - 프레임 모니터
