```
VAR
	BUTTON: BOOL
	PREV_BTN: R_TRIG
	LAMP: BOOL
VAR_END
```
---
```
PREV_BTN(CLK:=BUTTON);
```

- **의미:** `R_TRIG` 기능을 하는 `PREV_BTN`의 입력핀인 `CLK`(Clock)에 `BUTTON`의 현재 상태(TRUE 또는 FALSE)를 집어넣어라.
- 즉, 이 한 줄이 매 스캔마다 실행되면서 버튼이 안 눌려있다가(FALSE) 방금 눌렸는지(TRUE)를 감시합니다.

```
IF BTN_R.Q THEN
```

- **의미:** 만약 `BTN_R`의 출력핀인 `Q`가 TRUE라면 아래 코드를 실행해라.
- `R_TRIG`의 특성상 `CLK`에 들어오는 값이 FALSE에서 TRUE로 바뀌는 그 찰나의 순간(딱 1스캔 주기)에만 출력 `.Q`가 `TRUE`가 됩니다.