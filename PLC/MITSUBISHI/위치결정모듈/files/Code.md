
### MAIN
```Pascal
// [MAIN] 1. 전 축 상태 감시 및 PLC Ready 제어
// 모든 축이 정지 상태인지 확인 (예: 2축 기준)
// 전 축 정지 상태 초기 판정
bAllAxisIdle := TRUE;

// 0축부터 1축까지 검사 (축 개수가 늘어나면 상한 숫자만 변경)
FOR i := 0 TO 1 DO
    IF RD77_1.bnBusy[i] THEN
        bAllAxisIdle := FALSE;
        EXIT; // 하나라도 구동 중이면 더 볼 필요 없이 반복문 탈출
    END_IF;
END_FOR;

// ROM 쓰기 요구가 없을 때만 PLC Ready 및 서보 온 유지
RD77_1.bPLC_Ready := NOT bRomWriteReq;
RD77_1.bAllAxisServoOn := TRUE;


// [MAIN] 2. Flash ROM 쓰기 공용 시퀀스
// 외부 ROM 쓰기 버튼 입력 시 트리거
R_TRIG_MAIN_ROMWR(CLK := BTN_ROM_WRITE);

// 전 축 정지 상태일 때만 쓰기 시작 허용
IF R_TRIG_MAIN_ROMWR.Q AND bAllAxisIdle THEN
    bRomWriteReq := TRUE;
    bRomCmdSent  := FALSE;
END_IF;

// PLC_Ready가 완전히 떨어진 것 확인 후 ROM 쓰기 버퍼 On
IF bRomWriteReq AND (NOT RD77_1.bPLC_Ready) AND (NOT bRomCmdSent) THEN
    RD77_1.stSysCtrl_D.uWriteFlashRom_D := 1;
    bRomCmdSent := TRUE;
END_IF;

// 모듈이 ROM 쓰기 완료(버퍼가 0으로 자동 복귀)하면 요구 플래그 해제 -> PLC Ready 복구
IF bRomWriteReq AND bRomCmdSent AND (RD77_1.stSysCtrl_D.uWriteFlashRom_D = 0) THEN
    bRomWriteReq := FALSE;
    bRomCmdSent  := FALSE;
END_IF;


// [MAIN] 3. 각 축 FB 멀티 인스턴스 호출
fbAxis0(
N_AXIS := 0,
START_INIT := BTN_INIT_AX0,
HOMING := BTN_HOMING_AX0,
Point1 := BTN_P1_AX0,
Point2 := BTN_P2_AX0,
Point3 := BTN_P3_AX0,
CW := JOG_CW_AX0,
CCW := JOG_CCW_AX0
);

fbAxis1(
N_AXIS := 1,
START_INIT := BTN_INIT_AX1,
HOMING := BTN_HOMING_AX1,
Point1 := BTN_P1_AX1,
Point2 := BTN_P2_AX1,
Point3 := BTN_P3_AX1,
CW := JOG_CW_AX1,
CCW := JOG_CCW_AX1
);
```

### FB_Axis_Control
```pascal
// 1. 트리거 펄스 생성
R_TRIG_INIT(CLK := START_INIT);  // 수동 INIT 시작 버튼 트리거
R_TRIG_TEACHING(CLK := TEACHING);
R_TRIG_HOMING(CLK := HOMING);
R_TRIG_P1(CLK := Point1);
R_TRIG_P2(CLK := Point2);
R_TRIG_P3(CLK := Point3);

// 2. 축별 기본 제어 신호 설정 (해당 N_AXIS 전용 버퍼 제어)
// JOG 속도 기본 설정
RD77_1.stnAxCtrl1_D[N_AXIS].udJOG_Speed_D := 10000;

// 에러 리셋 및 축 정지 신호
RD77_1.stnAxCtrl1_D[N_AXIS].uResetAxisError_D.0 := ERROR_RST;
RD77_1.stnAxCtrl2_D[N_AXIS].uStopAxis_D.0 := AXIS_STOP;

// =========================================================================
// [INIT 시퀀스]: START_INIT 버튼 입력 시 시작 -> 1회전 -> 정지 -> 9001 원점복귀
// =========================================================================
CASE iInitStep OF
    // Step 0: 수동 기동 대기 (버튼 누름 감지)
    0:
	IF R_TRIG_INIT.Q AND RD77_1.bPLC_Ready AND (NOT RD77_1.bnBusy[N_AXIS]) THEN
		INIT_FLAG := FALSE; // 재초기화 대비 플래그 리셋
		dStartPos := RD77_1.stnAxMntr_D[N_AXIS].dActualPosition_D;
		iInitStep := 1;
	END_IF;

    // Step 1: 저속 JOG로 1회전 이상 안전 구동
    1:
	IF ABS(RD77_1.stnAxMntr_D[N_AXIS].dActualPosition_D - dStartPos) >= 20000 THEN
		iInitStep := 2; // 회전 조건 만족 시 정지 단계로
	END_IF;

    // Step 2: 모터 정지 확인
    2:
	IF NOT RD77_1.bnBusy[N_AXIS] THEN
		iInitStep := 3;
	END_IF;

    // Step 3: 데이터 세트 방식 원점 복귀(9001) 기동 요청
    3:
	uTargetPosNo := 9001;
	bStartReq    := TRUE;
        
	// 기동 시작(Busy ON) 확인 시 Step 4로 전이
	IF RD77_1.bnBusy[N_AXIS] THEN
		iInitStep := 4;
	END_IF;

    // Step 4: 원점 복귀 완료 확인 후 초기화 종료
    4:
	IF NOT RD77_1.bnBusy[N_AXIS] THEN
		INIT_FLAG := TRUE; // 초기화 완료 플래그 ON
		iInitStep := 10;   // 정상 운전 대기 모드로 전환
	END_IF;

    // Step 10: 초기화 완료 후 일반 대기 상태 (다시 버튼을 누르면 0으로 리셋 가능)
    10:
	IF R_TRIG_INIT.Q THEN
		iInitStep := 0; // 재초기화 요청 시 Step 0으로 복귀
	END_IF;
END_CASE;

// =========================================================================
// JOG 구동 제어 (초기화 중일 때와 평상시 구분)
// =========================================================================
RD77_1.stnAxCtrl2_D[N_AXIS].uStartForwardJOG_D.0 := (iInitStep = 1) OR (INIT_FLAG AND CCW);
RD77_1.stnAxCtrl2_D[N_AXIS].uStartReverseJOG_D.0 := INIT_FLAG AND CW;


// =========================================================================
// 3. 위치결정 기동 시퀀스 (INIT_FLAG 완료 후 수동 기동 Proxy 요청 취합)
// =========================================================================
IF INIT_FLAG AND NOT RD77_1.bnBusy[N_AXIS] THEN
    IF R_TRIG_HOMING.Q THEN
        uTargetPosNo := 9001;
        bStartReq    := TRUE;
		ELSIF R_TRIG_P1.Q THEN
        uTargetPosNo := 1;
        bStartReq    := TRUE;
		ELSIF R_TRIG_P2.Q THEN
        uTargetPosNo := 2;
        bStartReq    := TRUE;
		ELSIF R_TRIG_P3.Q THEN
        uTargetPosNo := 3;
        bStartReq    := TRUE;
    END_IF;
END_IF;


// =========================================================================
// [단일 출력 제어]: 실제 모듈 버퍼 및 기동 코일 단일 대입 (이중 코일 방지)
// =========================================================================
// (1) 기동 요청 시 버퍼에 위치 번호 반영
IF bStartReq THEN
    RD77_1.stnAxCtrl1_D[N_AXIS].uPositioningStartNo_D := uTargetPosNo;
END_IF;

// (2) 모듈이 기동하여 Busy가 ON되면 요청 플래그 리셋
IF RD77_1.bnBusy[N_AXIS] THEN
    bStartReq := FALSE;
END_IF;

// (3) 기동 코일 단일 대입
RD77_1.bnPositioningStart[N_AXIS] := bStartReq AND NOT RD77_1.bnBusy[N_AXIS];


// =========================================================================
// 4. Teaching 시퀀스 (RAM 반영)
// =========================================================================
IF INIT_FLAG AND R_TRIG_TEACHING.Q AND NOT RD77_1.bnBusy[N_AXIS] AND NOT bTeachingReq THEN
    RD77_1.stnAxCtrl1_D[N_AXIS].uTeachingDataSelection_D := 0;      // 0: 위치 어드레스 티칭
    RD77_1.stnAxCtrl1_D[N_AXIS].uTeachingPositioningDataNo_D := 1;  // 티칭 대상 No.1 지정 및 기동
    bTeachingReq := TRUE;
END_IF;

IF bTeachingReq AND (RD77_1.stnAxCtrl1_D[N_AXIS].uTeachingPositioningDataNo_D = 0) THEN
    bTeachingReq := FALSE;
END_IF;
```