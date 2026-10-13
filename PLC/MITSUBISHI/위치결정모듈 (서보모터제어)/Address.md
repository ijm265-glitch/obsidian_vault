위치결정모듈의 버퍼 메모리 주소에서 PLC가 데이터를 작성하거나 읽어오는 것으로 서보모터의 제어를 할 수 있다.
다음은 RD77MS2의 주요 버퍼 메모리 어드레스와 레이블을 보여준다.
모든 어드레스 정보는 [[위치결정모듈 Address|링크]] 참고

* **축 오프셋 규칙:** $n = \text{축 No.} - 1$ (1축: $n=0$, 2축: $n=1$)
* **I/O 기준:** 선두 I/O 주소가 `0000`일 때 기준
---
### 1. 모니터링 변수 (모듈 $\rightarrow$ PLC)
* **`RD77_1.bPLC_Ready` (BOOL)**
	* **디바이스:** **`Y0`** (PLC CPU 준비 완료 입출력 신호)

* **`RD77_1.bnBusy[N_AXIS]` (BOOL)**
	* **디바이스:** **`X10 + n`** (축별 운전 중 / Busy 신호)
	* 1축: `X10`, 2축: `X11`, 3축: `X12`, 4축: `X13`
	* (참고: 버퍼 메모리 내 축 스테이터스 `[Md.31]` 주소 `2417 + 100n`의 b13으로도 확인 가능)

* **`RD77_1.stnAxMntr_D[N_AXIS].dActualPosition_D` (DINT)**
	* **파라미터명:** `[Md.20]` 이송 현재값 (32비트)
	* **버퍼 메모리 주소:** **`2400 + 100n` / `2401 + 100n**`
	* (엔코더 기반 물리 실 현재값 `[Md.101]`인 경우: `2450 + 100n` / `2451 + 100n`)

- `RD77_1.stnAxMntr[N_AXIS].uCurrentPositioningIdentifier`
	* **파라미터명:** `[Md.47] 실행중 위치결정 데이터 : 운전 패턴 (Operation pattern)`
	* **버퍼 메모리 주소:** **`2438 + 100n

- `RD77_1.stnAxMntr[N_AXIS].wAxisOperationStatus`
	- **파라미터명:**`[Md.26] 축 동작 상태`
	- **버퍼 메모리 주소:** **`2409 + 100n`**
		- `0`: 대기 (Standby)
		- `1`: 정지 중 (Stopped)
		- `2`: 보간 중 (Interpolating)
		- `3`: JOG 운전 중 (JOG operation)
		- `4`: 수동 펄서 운전 중 (Manual pulser operation)
		- `5`: 원점 복귀 중 (OPR)
		- `6`: 위치결정 제어 중 (Positioning)		    
		- `-1`: 에러 발생 상태
---
### 2. 제어 지령 변수 (PLC $\rightarrow$ 모듈)
* **`RD77_1.bnPositioningStart[N_AXIS]` (BOOL)**
	* **디바이스:** **`Y10 + n`** (축별 위치결정 기동 신호)
	* 1축: `Y10`, 2축: `Y11`, 3축: `Y12`, 4축: `Y13`

* **`RD77_1.stnAxCtrl1_D[N_AXIS].uPositioningStartNo_D` (WORD / UINT)**
	* **파라미터명:** `[Cd.3]` 위치결정 기동 번호 (1~600, 9001 등)
	* **버퍼 메모리 주소:** **`4300 + 100n`**

* **`RD77_1.stnAxCtrl1_D[N_AXIS].udJOG_Speed_D` (UDINT)**
	* **파라미터명:** `[Cd.17]` JOG 속도 (32비트)
	* **버퍼 메모리 주소:** **`4318 + 100n` / `4319 + 100n**`

* **`RD77_1.stnAxCtrl1_D[N_AXIS].uResetAxisError_D.0` (BOOL, 비트 0)**
	* **파라미터명:** `[Cd.5]` 축 에러 리셋 (비트 0 ON 시 리셋)
	* **버퍼 메모리 주소:** **`4302 + 100n`**

* **`RD77_1.stnAxCtrl2_D[N_AXIS].uStopAxis_D.0` (BOOL, 비트 0)**
	* **파라미터명:** `[Cd.180]` 축정지 (비트 0 ON 시 감속 정지)
	* **버퍼 메모리 주소:** **`30100 + 10n`**

* **`RD77_1.stnAxCtrl2_D[N_AXIS].uStartForwardJOG_D.0` (BOOL, 비트 0)**
	* **파라미터명:** `[Cd.181]` 정회전 JOG 기동 (비트 0 ON)
	* **버퍼 메모리 주소:** **`30101 + 10n`**

* **`RD77_1.stnAxCtrl2_D[N_AXIS].uStartReverseJOG_D.0` (BOOL, 비트 0)**
	* **파라미터명:** `[Cd.182]` 역회전 JOG 기동 (비트 0 ON)
	* **버퍼 메모리 주소:** **`30102 + 10n`**

---

### 3. 티칭(Teaching) 관련 변수 (PLC $\rightarrow$ 모듈)

* **`RD77_1.stnAxCtrl1_D[N_AXIS].uTeachingDataSelection_D` (WORD / UINT)**
	* **파라미터명:** `[Cd.38]` 티칭 데이터 선택 (0: 실 현재값, 1: 지령 현재값)
	* **버퍼 메모리 주소:** **`4348 + 100n`**

* **`RD77_1.stnAxCtrl1_D[N_AXIS].uTeachingPositioningDataNo_D` (WORD / UINT)**
	* **파라미터명:** `[Cd.39]` 티칭 위치결정 데이터 No. (1~600 지정)
	* **버퍼 메모리 주소:** **`4349 + 100n`**