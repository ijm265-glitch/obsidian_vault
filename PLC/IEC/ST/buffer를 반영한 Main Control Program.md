오류가 발생하더라도 해당 모듈만 동작을 중지하도록 설정
버퍼를 반영하여 작업을 실행하도록 설정

```pascal
// ==========================================
// [모듈 1 제어 상태기 (버퍼 연동 및 독립 동작)]
// ==========================================
// 1. 버퍼 상태 업데이트 (상위 신호 bMain_SystemStart 및 센서/버퍼 조건 확인)
// 시스템이 가동 중이고, 모듈1 입력 버퍼에 자재가 존재하면 실행 요청 ON
IF bMain_SystemStart AND bBuf_Exist1 THEN
    bReq_Start1 := TRUE;
END_IF;

CASE nState_Mod1 OF
    0: // 모듈 1 대기
        bMod_CmdStart1 := FALSE;
        // 실행 요청이 있고, 에러가 없으며, 버퍼에 작업 대상이 있을 때 출발
        IF bReq_Start1 AND bBuf_Exist1 AND NOT fbModule1.bOut_Error THEN
            nState_Mod1 := 10;
        END_IF;

    10: // 모듈 1 가동 (Start 명령 전송)
        bMod_CmdStart1 := TRUE;
        IF fbModule1.bOut_Busy OR fbModule1.bOut_Done THEN
        // 모듈 1 응답 확인하면 
            nState_Mod1 := 20;
        END_IF;

    20: // 모듈 1 완료 대기 및 Handshake Clear
        IF fbModule1.bOut_Done THEN
            bMod_CmdStart1 := FALSE; // Start 신호 OFF -> FB 내부 Done OFF 유도
            nState_Mod1 := 30;
        END_IF;

    30: // Handshake 완료 확인 및 버퍼 상태 기반 연속 동작 판단
        IF NOT fbModule1.bOut_Done THEN
            // 버퍼가 없거나 시스템이 멈춘 상태면 요청 해제, 버퍼가 계속 남아있으면 bReq_Start1 유지
            IF NOT bBuf_Exist1 OR NOT bMain_SystemStart THEN
                bReq_Start1 := FALSE;
            END_IF;
            nState_Mod1 := 0; // State 0으로 돌아가 bReq_Start1 및 버퍼 유무 확인 후 즉시 재가동
        END_IF;

    999: // 모듈 1 개별 에러 처리
        bMod_CmdStart1 := FALSE;
        bReq_Start1    := FALSE;
        IF bMain_SystemReset THEN
            bMod_CmdReset1 := TRUE;
            nState_Mod1    := 0;
        END_IF;
END_CASE;

// 모듈 1 개별 에러 인터락
IF fbModule1.bOut_Error THEN
    nState_Mod1 := 999;
ELSE
    bMod_CmdReset1 := FALSE;
END_IF;


// ==========================================
// [모듈 2 제어 상태기 (버퍼 연동 및 독립 동작)]
// ==========================================
// 1. 버퍼 상태 업데이트
IF bMain_SystemStart AND bBuf_Exist2 THEN
    bReq_Start2 := TRUE;
END_IF;

CASE nState_Mod2 OF
    0: // 모듈 2 대기
        bMod_CmdStart2 := FALSE;
        IF bReq_Start2 AND bBuf_Exist2 AND NOT fbModule2.bOut_Error THEN
            nState_Mod2 := 10;
        END_IF;

    10: // 모듈 2 가동
        bMod_CmdStart2 := TRUE;
        IF fbModule2.bOut_Busy OR fbModule2.bOut_Done THEN
            nState_Mod2 := 20;
        END_IF;

    20: // 모듈 2 완료 대기 및 Handshake Clear
        IF fbModule2.bOut_Done THEN
            bMod_CmdStart2 := FALSE;
            nState_Mod2 := 30;
        END_IF;

    30: // Handshake 완료 확인 및 버퍼 상태 기반 연속 동작 판단
        IF NOT fbModule2.bOut_Done THEN
            IF NOT bBuf_Exist2 OR NOT bMain_SystemStart THEN
                bReq_Start2 := FALSE;
            END_IF;
            nState_Mod2 := 0;
        END_IF;

    999: // 모듈 2 개별 에러 처리
        bMod_CmdStart2 := FALSE;
        bReq_Start2    := FALSE;
        IF bMain_SystemReset THEN
            bMod_CmdReset2 := TRUE;
            nState_Mod2    := 0;
        END_IF;
END_CASE;

// 모듈 2 개별 에러 인터락
IF fbModule2.bOut_Error THEN
    nState_Mod2 := 999;
ELSE
    bMod_CmdReset2 := FALSE;
END_IF;
```

