## UART (Universal Asynchronous Receiver-Transmitter)
### 1. 비동기식 (Asynchronous) : 송신측과 수신측의 타이밍을 맞춰주는 Clock선이 없음
- 보드레이트 (Baud Rate) : 클럭이 없는 대신 1초에 몇 개의 비트를 보낼 것인가를 사전에 일치시켜야 함 이를 Baud Rate(bps : Bits Per Second)라고 한다
- 클럭 오차 (Clock Tolerance): 클럭 선이 없기 때문에 각 기기 내부의 타이머(오실레이터)의 의존도가 높고 두 기기간의 보드레이트 오차가 커짐녀 수신측이 브트를 읽는 타이밍이 밀리며 데이터가 깨짐

### 2. 하드웨어 연결과 전기적 특성
- TX는 상대방의 RX로, 내 RX는 상대방의 TX로 교차 연결함
- 두 기기의 접지를 서로 연결해서 기준전압을 설정해야 함
- ESP32에서 3.3V는 논리 1(High), 0V는 논리 0(Low)

### 3. UART 데이터 프레임 구조
##### Idle -> Start Bit -> Data  Bits -> (Parity Bit) -> Stop Bit -> Idle
- Idle : 통신을 하지 않을 떄 RX에 항상 High상태를 유지
- Start Bit (1Bit): 전압이 Low상태로 변하면 수신측에서 데이터 수신 준비를 하며 타이머를 작동시킴
- Data Bits (8Bits, 1Bytes) : 실제로 보낼 데이터, LSB(Least Significant Bit)부터 보냄
- Parity Bit (선택사항, 1Bit) : 전송중 데이터가 노이즈로 인해 깨졌는지 검사하는 오류 검출 비트
- Stop Bit (1 or 2Bits) : 데이터 전송이 끝났음을 알리기 위해 전압을 다시 High로 올려서 일정시간 유지

UART설정 예시 : 115200, 8, N, 1 (Baud Rate 115200, Data 8bits, Parity None, Stop 1bit)

# I2C (Inter-Intergrated Circuit)
## 동기식 및 주소 지정 방식 (Synchronous & Addressing)
- **동기식** : 송신측과 수신측의 동기화를 위한 Clock선이 존재함 
- **주소 지정 시스템** : 하나의 버스(선)에 여러 장치를 연결하는 멀티 드롭 구조이기 때문에 각 슬레이브 기기는 고유의 7or10비트 고유 주소를 가짐. 마스터가 통신 시작 시 주소를 먼저 쏘아 일치하는 장치와만 1:N 통신을 수행함
- **클럭 스트레칭** : 슬레이브의 데이터 처리 속도가 느릴 때 SCL 라인을 강제로 Low로 설정하여 마스터의 클럭 타이머를 일시 정지 시킴
## 하드웨어 연결과 전기적 특성
- **SCL(Serial Clock)과 SDA(Serial Data)** 단 2가닥의 선을 모든 기기가 공유하며, 마스터와 슬레이브 간에 같은 이름의 핀끼리 병렬로 직결함 (SCL은 SCL끼리, SDA는 SDA끼리).
- **오픈 드레인(Open-Drain) 구조:** 기기들이 선에 전기를 직접 밀어 넣지 못하고, 선을 0V(GND)로 끌어내리는 스위치 역할만 수행함. 여러 기기가 동시에 출력해도 쇼트(합선)가 나지 않는 구조.
- **풀업 저항(Pull-up Resistor):** 오픈 드레인 구조 특성상, 평소 버스가 대기 상태일 때 3.3V(High) 논리 레벨을 팽팽하게 유지해 주기 위해 전원(VCC)과 통신선 사이에 필수적으로 연결하는 부하 저항.
## I2C 데이터 프레임 구조 (I2C Data Frame Architecture)

 **Idle $\rightarrow$ START Condition $\rightarrow$ Address Frame (7-bit) $\rightarrow$ R/W Bit (1-bit) $\rightarrow$ ACK/NACK (1-bit) $\rightarrow$ Data Frame (8-bit) $\rightarrow$ ACK/NACK (1-bit) $\rightarrow$ STOP Condition $\rightarrow$ Idle
- **Idle (대기 상태)**
    - **전기적 상태:** 풀업 저항에 의해 SCL(클럭)과 SDA(데이터) 선이 모두 **High(3.3V)** 레벨을 팽팽하게 유지하고 있는 상태야. 버스가 비어있음을 의미해.
- **START Condition (시작 조건)**
    - **동작 원리:** 통신의 시작을 알리는 신호야. SCL이 **High**를 유지하고 있는 상태에서, 마스터가 **SDA 라인을 High에서 Low로 먼저 떨어뜨리는 순간(Falling Edge)** 통신이 시작된 것으로 판정해. 모든 슬레이브는 이 순간부터 잠에서 깨어나 클럭을 맞이할 준비를 하지.
- **Address Frame (슬레이브 주소 지정, 7-Bits)**
    - **동작 원리:** 마스터가 대화하고 싶은 슬레이브의 고유 이름표(주소)를 7비트 크기로 쏴주는 구간이야.
    - **타이밍 특징:** 앞서 배운 대로 SCL 클럭이 상승 에지(Low $\rightarrow$ High)일 때 SDA의 전압 레벨을 읽어서 `0`과 `1`을 판별해. 최상위 비트(**MSB**, Most Significant Bit)부터 차례대로 전송돼.
- **R/W Bit (읽기/쓰기 정의, 1-Bit)**
    - **동작 원리:** 주소 뒤에 붙는 8번째 비트야. 마스터가 슬레이브에게 데이터를 **보낼 것인지(Write = 0, Low)**, 아니면 슬레이브로부터 데이터를 읽어올 것인지(Read = 1, High)를 결정해 주는 전기적 스위치 역할을 해.
- **ACK / NACK (응답 확인, 1-Bit)**
    - **동작 원리:** 주소와 R/W 비트까지 총 8비트가 전달되면, 9번째 SCL 클럭 주기 동안 마스터는 SDA 선을 제어하지 않고 손을 떼(High 상태로 방치).
    - **판별:** 이때 주소가 일치하는 슬레이브가 정상적으로 준비되었다면 SDA 선을 강제로 Low로 꾹 끌어내리는데, 이를 ACK(Acknowledge)라고 해. 만약 선이 High 그대로 붕 떠 있다면 NACK로 판단하고 통신을 중단하거나 재시도하게 돼.
- **Data Frame (실제 데이터 전송, 8-Bits / 1-Byte)**
    - **동작 원리:** 주소가 매칭되어 통로가 열렸으니 진짜 데이터를 주고받는 구간이야. Address 프레임과 마찬가지로 **MSB부터** 시작해 1바이트(8비트)씩 전송되며, SCL 클럭의 상승 에지 타이밍에 데이터를 샘플링해.
- **ACK / NACK (데이터 응답, 1-Bit)**
    - **동작 원리:** 데이터를 1바이트 보낼 때마다 9번째 클럭에서 수신측(보낼 때는 슬레이브, 읽을 때는 마스터)이 "방금 1바이트 잘 받았어!"라는 의미로 SDA를 Low(ACK)로 떨어뜨려 줘.
- **STOP Condition (종료 조건)**
    - **동작 원리:** 대화가 끝났음을 공표하는 신호야. SCL 라인이 **High** 상태를 유지하고 있는 전위에서, 마스터가 **SDA 라인을 Low에서 High로 끌어올리는 순간(Rising Edge)** 통신이 최종 종료돼. 이 신호가 나오면 버스는 다시 모두가 쓸 수 있는 대기(Idle) 상태로 돌아가.






# SPI (Serial Peripheral Interface)

## 1. 동기식 및 전이중 고속 통신 (Synchronous & Full-Duplex) 
### 마스터가 출력하는 Clock 신호에 맞춰 송신선(MOSI)과 수신선(MISO)을 통해 동시에 데이터를 교환함
- **시프트 레지스터 맞교환 (Shift Register Swap) :** 통신의 본질이 마스터와 슬레이브 내부의 메모리를 하나의 고리 형태로 결합해 비트를 맞교환하는 구조임. 대장이 데이터를 읽기만 원할 때도 의미 없는 더미 데이터(Dummy Data)를 의무적으로 송신해야 박자(SCK)가 뛰어 슬레이브의 데이터가 밀려 들어옴
- **통신 모드 (SPI Modes) :** 클럭의 기본 극성(CPOL)과 데이터 샘플링 위상(CPHA)의 조합에 따라 4가지 모드(Mode 0~3)로 나뉘며, 두 기기의 설정을 반드시 일치시켜야 데이터 오독이 없음
## 2. 하드웨어 연결과 전기적 특성

- MOSI, MISO, SCK 공통 버스선과 슬레이브 장치 개수만큼의 독립된 CS 선이 필요하여 기본적으로 4가닥+의 선으로 연결함
- 핀맵에 표시된 기본 CS 핀 외에, 보드에 남아도는 일반 GPIO 핀도 얼마든지 특정 기기의 CS(스위치) 핀으로 할당하여 1:N 병렬 연결이 가능함
- 푸시풀(Push-Pull) 출력 구조를 사용하여 내부 트랜지스터가 3.3V(High)와 0V(Low) 전압을 능동적으로 강하게 밀고 당기므로 풀업 저항이 필요 없고 수십 MHz 단위의 초고속 전송이 가능함
## 3. SPI 데이터 프레임 구조
- CS High (Idle) $\rightarrow$ CS Low (Assert) $\rightarrow$ SCK Clock & Data Swap $\rightarrow$ CS High (De-assert) $\rightarrow$ CS High (Idle)
- **Idle :** 통신을 하지 않을 때 CS 라인은 항상 High 상태를 유지하여 슬레이브들을 비활성화 상태로 대기시키며, SCK는 CPOL 설정에 따른 기본 전압 레벨을 유지함
- **CS Assertion (장치 선택) :** 마스터가 대화하고자 하는 특정 슬레이브의 CS 라인을 Low(0V)로 떨어뜨려 칩을 활성화함 (I2C처럼 주소를 방송하는 과정이 없어 오버헤드가 없음)
- **SCK Clock & Data Swap (8Bits) :** CS가 Low가 된 순간부터 마스터가 사각파 클럭을 발생시킴. 모드 0 기준, 평소 0V를 유지하다가 3.3V로 전압이 급격히 올라가는 첫 번째 상승 에지(Rising Edge) 찰나에 MOSI와 MISO 선 위의 데이터를 동시에 읽어 들이며, 일반적으로 최상위 비트(MSB)부터 전송됨
- **CS De-assertion (통신 종료) :** 정해진 데이터 교환이 완료되면 마스터는 클럭 공급을 중단하고, 해당 CS 라인을 다시 High로 올려서 슬레이브를 대기 상태로 되돌림
- **SPI설정 예시 :** Mode 0, MSB First (CPOL=0, CPHA=0 / 평소 클럭은 Low 유지, 첫 번째 상승 에지 순간에 데이터를 샘플링하며 최상위 비트부터 전송함)