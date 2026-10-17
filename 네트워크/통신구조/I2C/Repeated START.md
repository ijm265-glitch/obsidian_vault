Repeated START(반복 시작, 줄여서 `Sr`)는 I2C 통신에서 "쓰기 동작을 마치고 읽기 동작으로 넘어갈 때, 버스 제어권을 내려놓지 않고 원자적(Atomic)으로 작업을 이어가기 위한 핵심 메커니즘"입니다.

이 동작이 왜 필요하고, 어떻게 일어나는지 3가지 관점으로 나누어 설명합니다.

---

### 1. 왜 STOP을 거치지 않는가? (버스 점유권 유지의 이유)

일반적인 통신 방식대로라면 아래처럼 동작해야 할 것 같습니다.

1. "0x3B 레지스터로 이동해라" 쓰고 $\rightarrow$ **STOP(통신 종료)**
2. "이제 그 레지스터 값 읽겠다" $\rightarrow$ **START(새 통신 시작)**

하지만 여기서 **STOP을 보내면 큰 문제 2가지**가 생길 수 있습니다.

* **멀티 마스터 환경에서의 버스 가로채기 방지:**
I2C 버스 라인에는 마스터 장치(MCU)가 2개 이상 물릴 수 있는 멀티 마스터(Multi-master) 환경이 가능합니다. 만약 1번 마스터가 STOP을 날리는 순간 I2C 버스는 유휴 상태(Idle: SDA/SCL 모두 HIGH)가 됩니다. 이때 다른 마스터가 "버스가 비었네?" 하고 끼어들어 버스를 가로채면, 원래 진행하려던 1번 마스터의 읽기 작업이 엉망이 됩니다.
* **센서 내부 상태(포인터) 초기화 방지:**
일부 정밀 센서나 메모리 칩은 STOP 신호를 감지하면 통신 세션이 끝난 것으로 판단하여 내부 레지스터 포인터를 기본값(0x00 등)으로 리셋해 버리는 경우가 있습니다.

따라서 "방금 설정한 레지스터 포인터를 그대로 유지한 채, 아무도 끼어들지 못하게 버스 점유권을 쥔 상태로 즉시 모드만 쓰기에서 읽기로 바꾸겠다"라는 약속이 바로 **Repeated START**입니다.

---

### 2. 전기 신호 레벨에서 일어나는 일 (파형 원리)

I2C 규약에서 통신의 시작과 끝은 오직 **SCL 클록이 HIGH로 떠 있을 때 SDA 핀의 전압 변화**로만 판단합니다.

* **START (`S`) :** SCL이 HIGH일 때 SDA가 **HIGH $\rightarrow$ LOW**로 떨어짐
* **STOP (`P`) :** SCL이 HIGH일 때 SDA가 **LOW $\rightarrow$ HIGH**로 올라감

**Repeated START (`Sr`)의 파형 흐름:**

1. 레지스터 주소 전송 후 슬레이브로부터 9번째 클록에서 ACK 응답을 받습니다.
2. 마스터는 통신을 끝내는 **STOP(SDA를 LOW에서 HIGH로 올리는 신호)을 발생시키지 않습니다.**
3. 대신 SCL이 LOW인 틈을 타서 SDA를 몰래 다시 HIGH 상태로 띄워둡니다.
4. 그 상태에서 SCL을 HIGH로 올린 뒤, SDA를 다시 **HIGH $\rightarrow$ LOW**로 쾅 떨어뜨립니다.
5. 버스에 매달린 슬레이브들 입장에선 **"STOP 신호는 본 적도 없는데, 또 한 번 START 신호가 들어왔네?"** 하고 인식하게 됩니다. 이것이 '반복 시작(Repeated START)'입니다.

---

### 3. 우리가 작성했던 코드와의 매핑

우리가 `Wire.h`로 작성했던 코드의 바로 이 한 줄이 Repeated START를 직접 제어하는 스위치였습니다.

```cpp
Wire.beginTransmission(MPU_ADDR); // [START] 발생
Wire.write(0x3B);                 // 포인터 보냄

// 핵심: 인자로 false를 넘김!
byte status = Wire.endTransmission(false); // [STOP 생략, 버스 유지]

// 바로 이어서 요청
Wire.requestFrom((uint16_t)MPU_ADDR, (uint8_t)14, true); // [Repeated START 발생]

```

* **`Wire.endTransmission(true)` (기본값):** 버스에 STOP 조건을 띄워 통신을 완전히 끝내고 버스 선로를 놓아줍니다.
* **`Wire.endTransmission(false)`:** STOP 신호를 **보내지 않고 대기(Restart 대기)** 상태로 버스를 붙잡고 있습니다.
* 바로 다음 줄의 `Wire.requestFrom(...)`이 실행되면서 하드웨어 내부적으로 **Repeated START 신호**를 쏘고, 읽기 모드 바이트(`0xD1`)를 즉시 연속해서 날리게 됩니다.