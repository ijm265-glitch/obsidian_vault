
### SITA
- **상태 (State):** 시스템이 머무를 수 있는 조건이나 모드 (예: `Idle`, `Attack`, `Die`).
- **입력/이벤트 (Input/Event):** 상태 변화를 일으키는 자극 (예: `EnemyInRange`, `TakeDamage`).
- **전이 (Transition):** 현재 상태에서 특정 입력을 받아 다른 상태로 넘어가는 규칙.
- **동작 (Action):** 상태 진입(`OnEnter`), 유효 상태 동안 수행(`OnUpdate`), 탈출(`OnExit`) 시 일어나는 동작.

## Example
> 버튼을 누르면 대기중이던 3개의 램프가 순차적으로 2초간격으로 켜진다.
> 버튼을 한번 더 누르면 전부 꺼진다.

- 상태: 대기상태, 작동상태
- 입력: 버튼을 누름
- 전이: 대기상태에서 작동상태로 이동, 작동상태에서 대기상태로 이동
- 동작: 각각의 상태에서 어떻게 동작할 지

```pascal
VAR
    mode     : WORD; // 0: IDLE, 1: RUNNING
    btn0     : BOOL;
    lamp0    : BOOL;
    lamp1    : BOOL;
    lamp2    : BOOL;
    ton0     : TON;
    ton1     : TON;
    r_trig_0 : R_TRIG;
END_VAR;

// 1. INPUT (상승 엣지 감출)
r_trig_0(CLK := btn0);

// 2. TRANSITION (상태 전이) - ELSIF를 써야 동일 스캔 이중 전이를 방지함
IF r_trig_0.Q THEN
    IF mode = 0 THEN
        mode := 1;
    ELSIF mode = 1 THEN
        mode := 0;
    END_IF;
END_IF;

// 3. TIMER LOGIC (타이머 연속 구동)
ton0(IN := (mode = 1), PT := T#2S);
ton1(IN := ton0.Q,     PT := T#2S); // ton0이 완료되면 ton1 시작

// 4. ACTION (상태별 동작 출력)
CASE mode OF
    0:
        lamp0 := FALSE;
        lamp1 := FALSE;
        lamp2 := FALSE;

    1:
        lamp0 := TRUE;      // 시작 즉시 ON
        lamp1 := ton0.Q;    // 2초 후 ON
        lamp2 := ton1.Q;    // 4초 후 ON (2초 + 2초)
END_CASE;




```