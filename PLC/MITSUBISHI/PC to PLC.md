Ethernet Cable을 사용하여 [[SLMP(MC Protocol)]]를 사용한다.
PC Client
PLC Server로 동작
Q PLC의 경우 Modbus TCP를 지원하지 않으므로 SLMP를 이용하여야 함 - Modbus TCP를 사용하기 위해 별도 모듈을 이용할 수 있으나 불필요함

[[Connection Destination Ethernet 설정]]

# 1. 파라미터 세팅 
PLC Parameter - Built-in Ethernet Port Setting
![[Pasted image 20260930102304.png]]

[[2.6 ARP 프로토콜]]
**IP Address** 
- PLC의 IP Address를 설정한다. Q PLC의 Default IP는 `192.168.3.39`이다


**Subnet Mask Pattern**
- IP Address의 어디까지가 Subnet인지를 가려내기 위해 작성. 
- `255.255.255.0`

**Default Router IP Address** 
- 다른 네트워크 대역 (서브넷)끼리 통신하는 경우 라우터(게이트웨이)의 Address를 작성한다. `ex). 192.168.3.9와 192.168.2.4가 통신하는 경우 서브넷이 다르기 때문에 라우터를 거쳐야 함`
- 라우터를 사용하지 않아도 가상 게이트웨이인 192.168.3.1을 사용한다.

**Enable Online Change** : PC에서 읽기만이 아닌 쓰기도 수행하기 위해 체크

![[Pasted image 20260930110244.png]]
**Open Setting**
- Built-in Ethernet Port Open Setting으로, 해당 IP의 Port를 최대 16개 설정한다.
- TCP를 사용하고 MC Protocol이며 Host Station Port No. (PLC Port)를 5010으로 설정하였다.


# PC Client 생성

```python
import pymcprotocol

# Q 시리즈 내장 이더넷은 기본적으로 Type3E(3E 프레임)를 사용합니다.
mc = pymcprotocol.Type3E()
mc.setaccessopt(commtype="binary")  # Open Setting에서 지정한 Binary 방식

PLC_IP = "192.168.3.39"
PLC_PORT = 5010  # Open Setting에서 설정한 포트 번호

try:
    print(f"Connecting to PLC ({PLC_IP}:{PLC_PORT})...")
    mc.connect(PLC_IP, PLC_PORT)
    print("✅ PLC Connected Successfully!\n")

    # ---------------------------------------------------------
    # 1. D영역(워드 데이터) 쓰기 및 읽기 테스트
    # ---------------------------------------------------------
    # D100에 1234, D101에 5678 쓰기
    write_data = [1234, 5678]
    mc.batchwrite_wordunits(headdevice="D100", values=write_data)
    print(f"[Write] D100 ~ D101 <- {write_data}")

    # D100부터 2개 워드 읽기
    read_data = mc.batchread_wordunits(headdevice="D100", readsize=2)
    print(f"[Read]  D100 ~ D101 -> {read_data}\n")

    # ---------------------------------------------------------
    # 2. M영역(비트 접점) 쓰기 및 읽기 테스트
    # ---------------------------------------------------------
    # M100: ON(1), M101: OFF(0), M102: ON(1)
    bit_data = [1, 0, 1]
    mc.batchwrite_bitunits(headdevice="M100", values=bit_data)
    print(f"[Write] M100 ~ M102 <- {bit_data}")

    # M100부터 3개 비트 읽기
    read_bits = mc.batchread_bitunits(headdevice="M100", readsize=3)
    print(f"[Read]  M100 ~ M102 -> {read_bits}")

except Exception as e:
    print(f"❌ Error occurred: {e}")

finally:
    mc.close()
    print("\nConnection closed.")
```