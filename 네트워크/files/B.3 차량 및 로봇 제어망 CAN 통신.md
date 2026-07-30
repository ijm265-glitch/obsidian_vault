자동차, 항공기, 그리고 로봇 제어 시스템의 중추가 되는 **CAN(Controller Area Network) 통신**의 핵심 구조, 노이즈 저항성, 그리고 충돌 없이 메시지 우선순위를 가리는 **중재(Arbitration) 메커니즘**입니다.

## 1. [[CAN 버스의 기본 구조]]와 특징

1980년대 보쉬(Bosch)에서 자동차 복잡한 배선을 줄이기 위해 개발한 **멀티마스터(Multi-Master) 방식의 시리얼 통신 규격**입니다.
```
[ CAN 버스 구조 (2선식 차동 버스) ]

 Node A (ECU)         Node B (ECU)         Node C (ECU)
┌──────────────┐     ┌──────────────┐     ┌──────────────┐
│  Controller  │     │  Controller  │     │  Controller  │
│  Transceiver │     │  Transceiver │     │  Transceiver │
└──────┬───────┘     └──────┬───────┘     └──────┬───────┘
       │ CAN_H              │ CAN_H              │ CAN_H
 ──────┴────────────────────┴────────────────────┴────── (120Ω 종단)
 ──────┬────────────────────┬────────────────────┬────── (120Ω 종단)
       │ CAN_L              │ CAN_L              │ CAN_L
```

- **메시지 중심 프로토콜 (Message-Oriented):** 특정 수신자 주소(Address)가 없습니다. 데이터를 송신할 때 "이 데이터가 무슨 데이터인가"를 나타내는 ID(Identifier)를 붙여 버스 전체에 브로드캐스팅합니다.
    
- **종단 저항 (Termination Resistor):** 버스 양 끝단에 **$120\,\Omega$ 저항**을 달아 신호의 반사(Reflection) 파형을 흡수 제거합니다.
    
- **논리 비트 구분 (Dominant vs Recessive):**
    
    - **Dominant (우선/우세 비트 - Logic 0):** 버스 전압 차이가 발생하는 상태 ($V_{\text{DIFF}} \approx 2.0\text{V}$)
        
    - **Recessive (열성 비트 - Logic 1):** 버스 전압 차이가 없는 상태 ($V_{\text{DIFF}} \approx 0\text{V}$)
        
    - **핵심:** Dominant(0)와 Recessive(1)가 충돌하면 **Dominant(0)가 항상 승리**하여 버스를 점유합니다. (AND 연산과 유사)
        

## 2. CAN 통신의 강력한 노이즈 저항성

엔진룸이나 로봇 모터 고전압 노이즈 환경에서도 CAN 통신이 안정적인 물리적·소프트웨어적 이유입니다.

### 1) 차동 전압(Differential Signaling) 및 동상 노이즈 상쇄

RS-485와 마찬가지로 CAN High($CAN\_H$)와 **CAN Low($CAN\_L$)** 2선을 꼬아서(Twisted Pair) 사용합니다.

```
  [ Recessive State (Logic 1) ]
  CAN_H = 2.5V,  CAN_L = 2.5V  ──►  V_DIFF = CAN_H - CAN_L = 0V

  [ Dominant State (Logic 0) ]
  CAN_H = 3.5V,  CAN_L = 1.5V  ──►  V_DIFF = CAN_H - CAN_L = 2.0V
```

- 외부에서 전자기파 노이즈가 유입되어 두 전선 전압이 동시에 $+5\text{V}$씩 뛰더라도, 수신측 차동 앰프에서 $V_{\text{DIFF}} = (CAN\_H + 5) - (CAN\_L + 5) = CAN\_H - CAN\_L$ 계산을 거치며 **동상 노이즈(Common-Mode Noise)가 완전히 제거**됩니다.
    

### 2) 하드웨어 차원의 에러 검출 및 격리 (Fault Confinement)

- **Bit Stuffing:** 동일한 비트가 5개 연속되면, 반대 비트를 1개 강제로 삽입하여 비동기 통신 시계(Clock) 동기를 유지하고 라인 이상을 감지합니다.
    
- **CRC (Cyclic Redundancy Check):** 패킷의 무결성을 $15\text{bit}$ CRC로 엄격히 검증합니다.
    
- **Fault Confinement (에러 고립):** 특정 노드가 고장 나 연속으로 에러를 내뿜으면, 해당 노드가 스스로 Bus-Off 상태(자가 격리)로 전환되어 전체 네트워크가 죽는 것을 막습니다.
    

## 3. 메시지 ID 기반 우선순위 중재 (Arbitration) 방식

CAN 통신에는 데이터를 관장하는 Master가 없습니다. 여러 노드(ECU)가 동시에 버스에 메시지를 쏘기 시작할 때, **데이터 충돌이나 데이터 손실 없이 우선순위가 높은 메시지를 가려내는 기법**을 CSMA/AMP (Car-Sense Multiple Access with Bitwise Arbitration)라고 합니다.

### 1) 비트 단위 중재 과정 (Bitwise Arbitration)

모든 노드는 자신이 비트를 쏘는 동시에, **자신이 쏜 비트와 실제 버스의 전압(비트) 상태가 일치하는지 모니터링**합니다.

```
[ 상황: Node A(ID: 0x0C4)와 Node B(ID: 0x0C8)가 동시에 전송 시작 ]

이진수 변환 (11-bit ID):
  Node A : 000 1100 0100 (ID: 0x0C4)
  Node B : 000 1100 1000 (ID: 0x0C8)

비트 순서:  bit10  bit9  bit8  bit7  bit6  bit5  bit4  bit3  bit2  bit1  bit0
Node A  :    0     0     0     1     1     0     0     0     1     0     0
Node B  :    0     0     0     1     1     0     0     1     ... (패배!)
                                                     ▲
                                         [ bit 3 위치에서 충돌 발생 ]
```

1. **bit 10 ~ bit 4 구간:**
    
    - Node A와 Node B 모두 동일한 비트(`0`, `0`, `0`, `1`, `1`, `0`, `0`)를 출력하므로 충돌이 발생하지 않고 함께 진행됩니다.
        
2. **bit 3 구간 (결정적 순간):**
    
    - **Node A**는 `0` (Dominant)을 버스에 출력합니다.
        
    - **Node B**는 `1` (Recessive)을 버스에 출력합니다.
        
    - 버스의 실제 전압은 Dominant(0)의 승리로 `0`이 됩니다.
        
3. **Node B의 패배 인식 및 즉시 퇴장:**
    
    - Node B는 "나는 `1`을 쐈는데 버스 모니터링 결과 `0`이 찍히네?"라고 감지하는 순간, 자신이 우선순위 경쟁에서 졌음을 인정하고 송신을 즉시 중단(Arbitration Lost)합니다.
        
    - Node B는 수신 모드로 전환되어 Node A의 메시지를 조용히 수신합니다.
        
4. **Node A의 끊김 없는 전송:**
    
    - Node A는 자신이 쏜 `0`이 그대로 유지되었으므로 **재전송이나 지연 없이(Zero Delay) 완벽하게 남은 데이터까지 전송을 완료**합니다.
        

### 💡 핵심 요약

- **ID 번호가 작을수록 우선순위가 높다:** 이진수로 변환했을 때 앞자리부터 **`0` (Dominant) 비트가 더 일찍/많이 나오는 ID가 우선순위 경쟁에서 승리**합니다. (예: 브레이크/에어백 메시지 ID `0x001` > 오디오 조절 메시지 ID `0x7FF`)
    
- **지연 시간 없는 충돌 해결:** 이더넷(CSMA/CD)처럼 충돌 후 둘 다 멈추고 랜백오프 시간을 기다리는 것이 아니라, **우선순위가 높은 메시지는 0.001초의 지연도 없이 즉시 버스를 점유**하므로 실시간성이 요구되는 자동차/로봇 제어에 완벽히 부합합니다.