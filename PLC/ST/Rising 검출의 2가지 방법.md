
1. 이전 스캔에서의 버튼의 상태와 현재의 버튼의 상태를 비교하여 Rising Edge를 검출한다.
```
VAR
	BUTTON: BOOL
	PREV_BTN: BOOL
	LAMP: BOOL
VAR_END
```
```C
IF BUTTON = TRUE AND PREV_BTN = FALSE THEN // RISING 검출
	LAMP := NOT LAMP
	
END_IF

PREV_BTN := BUTTON;
```
2. **[[R_TRIG]]** 함수를 활용한다
```
VAR
	BUTTON: BOOL
	PREV_BTN: R_TRIG
	LAMP: BOOL
VAR_END
```
```C
PREV_BTN(CLK:=BUTTON);

IF PREV_BTN.Q THEN
	LAMP := NOT LAMP;
```