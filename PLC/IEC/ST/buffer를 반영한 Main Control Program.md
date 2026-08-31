오류가 발생하더라도 해당 모듈만 동작을 중지하도록 설정
버퍼를 반영하여 작업을 실행하도록 설정

```pascal
// ==========================================
// [Function Block 인스턴스 호출]
// ==========================================
fbModule1(
	bCmd_Start     := bMod_CmdStart1, 
	bCmd_Stop      := bMod_CmdStop1 OR bMain_SystemStop, 
	bCmd_Reset     := bMod_CmdReset1 OR bMain_SystemReset, // 전체 리셋 신호 직결
	bCmd_Auto      := bMod_CmdAuto1, 
	bSens_Home     := i_bSens_Home1, 
	bSens_Workdone := i_bSens_WorkDone1
);

fbModule2(
	bCmd_Start     := bMod_CmdStart2, 
	bCmd_Stop      := bMod_CmdStop2 OR bMain_SystemStop, 
	bCmd_Reset     := bMod_CmdReset2 OR bMain_SystemReset, // 전체 리셋 신호 직결
	bCmd_Auto      := bMod_CmdAuto2, 
	bSens_Home     := i_bSens_Home2, 
	bSens_Workdone := i_bSens_WorkDone2
);


// ==========================================
// [마스터 전체 리셋 인터락 (최상단 배치)]
// ==========================================
// 리셋 버튼 인가 시 동작 중이든 에러 상태든 상관없이 모든 상태기와 명령 변수를 대기 상태로 강제 초기화
IF bMain_SystemReset THEN
    // 모듈 1 상태 및 명령 초기화
    nState_Mod1    := 0;
    bMod_CmdStart1 := FALSE;
    bReq_Start1    := FALSE;
    
    // 모듈 2 상태 및 명령 초기화
    nState_Mod2    := 0;
    bMod_CmdStart2 := FALSE;
    bReq_Start2    := FALSE;
END_IF;


// ==========================================
// [모듈 1 제어 상태기]
// ==========================================
IF bMain_SystemStart AND i_bSens_buffer1 THEN
    bReq_Start1 := TRUE;
END_IF;

CASE nState_Mod1 OF
    0: // 모듈 1 대기
        bMod_CmdStart1 := FALSE;
        // 실행 요청, 센서 감지, 에러 없음 조건 만족 시 가동모드로 전환
        IF bReq_Start1 AND i_bSens_buffer1 AND NOT fbModule1.bOut_Error THEN
            nState_Mod1 := 10;
        END_IF;

    10: // 모듈 1 가동 (Start 명령 전송)
        bMod_CmdStart1 := TRUE; // 실제로 모듈을 실행하는 조건
        IF fbModule1.bOut_Busy OR fbModule1.bOut_Done THEN // 모듈이 시작 명령을 받고 Busy를 TRUE로 만들거나 이미 작업을 끝냈으면 
            nState_Mod1 := 20;
        END_IF;

    20: // 모듈 1 완료 대기 및 Handshake Clear
        IF fbModule1.bOut_Done THEN // 작업을 완료하면 
            bMod_CmdStart1 := FALSE; // Start 신호 OFF -> FB 내부 Done OFF 유도
            nState_Mod1 := 30;
        END_IF;

    30: // Handshake 완료 확인 및 연속 동작 판단
        IF NOT fbModule1.bOut_Done THEN // Done OFF가 확인되면
            // 버퍼 센서가 OFF이거나 시스템 정지 시 요청 해제
            IF NOT i_bSens_buffer1 OR NOT bMain_SystemStart THEN // 버퍼가 없거나 시스템 시작 명령이 없으면
                bReq_Start1 := FALSE; // 모듈1 시작요청 해제 -> 추후에 버퍼가 채워지면 27번 라인의 조건문에 의해 요청이 다시 TRUE로 전환
            END_IF;
            nState_Mod1 := 0; // State 0으로 돌아가 즉시 재가동 판단
        END_IF;

    999: // 모듈 1 개별 에러 처리
        bMod_CmdStart1 := FALSE;
        bReq_Start1    := FALSE;
        END_IF;
END_CASE;

// 모듈 1 개별 에러 인터락
IF fbModule1.bOut_Error THEN // 어떤 State에 있든 상관없이 에러 발생시 nState_Mod := 999로 전환
    nState_Mod1 := 999;
END_IF;

// ==========================================
// [모듈 2 제어 상태기]
// ==========================================
IF bMain_SystemStart AND i_bSens_buffer2 THEN
    bReq_Start2 := TRUE;
END_IF;

CASE nState_Mod2 OF
    0: // 대기
        bMod_CmdStart2 := FALSE;
        IF bReq_Start2 AND i_bSens_buffer2 AND NOT fbModule2.bOut_Error THEN
            nState_Mod2 := 10;
        END_IF;

    10: // 가동
        bMod_CmdStart2 := TRUE;
        IF fbModule2.bOut_Busy OR fbModule2.bOut_Done THEN
            nState_Mod2 := 20;
        END_IF;

    20: // 완료 대기
        IF fbModule2.bOut_Done THEN
            bMod_CmdStart2 := FALSE;
            nState_Mod2 := 30;
        END_IF;

    30: // Handshake 완료
        IF NOT fbModule2.bOut_Done THEN
            IF NOT i_bSens_buffer2 OR NOT bMain_SystemStart THEN
                bReq_Start2 := FALSE;
            END_IF;
            nState_Mod2 := 0;
        END_IF;

    999: // 개별 에러 대기
        bMod_CmdStart2 := FALSE;
        bReq_Start2    := FALSE;
END_CASE;

// 에러 발생 시 에러 상태 전환
IF fbModule2.bOut_Error THEN
    nState_Mod2 := 999;
END_IF;
```

