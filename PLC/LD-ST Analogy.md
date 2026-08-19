> 분명한 Analogy가 존재하지만 
> ST는 ST의 방식 (Finite State Machine based) 으로 작성하는것이 가독성이 좋고 오류를 적게 발생시키는 코드를 구성할 수 있음

## 직렬, 병렬 연결
![[Pasted image 20260811103619.png]]
```python
if p0 or p1:
	p20 = True
else:
	p20 = False
	
if po and p1:
	p21 = True
else:
	p21 = False
```
1. **기본적으로 False로 초기화 되어있으며 입력디바이스(p0, 01)는 임의로 변경 가능한 값이다.**
2. **크게 `while True:`로 감싸져 있다고 생각하자.**
3. **래더의 한 렁은 독립적인 조건문 `if, else`과 같다.** 
4. **병렬 연결은 조건문의 or에 해당하며, 직렬 연결은 조건문의 and에 해당한다.**


## 자기유지
![[Pasted image 20260811104223.png]]![[Pasted image 20260811105208.png]]

```python
if (p0 and not p1) or (p20 and not p1):
	p20 = True
else:
	p20 = False
```

```python
if p0:
	m0 = True

if p1:
	m0 = False

if m0:
	p20 = True
else:
	p20 = False
```
**`set, reset`을 사용하는 렁은 `else`가 없음**
`reset`을 활성화하는 입력에 대해서 `set (M0)`를 다시 `False`로 바꿀 수 있음을 의미한다.

## 2개의 입력으로 같은 동작 
![[Pasted image 20260811105744.png]]```
```python
if p1 or p2:
	p10 = True
	
if p3:
	p10 = False
```
`set`을 사용하는건 `else`를 사용하지 않는다는것을 의미
## 후입력 우선
![[Pasted image 20260811110700.png]]

```python
if p0:
	p20 = True
	p21 = False

if p1:
	p20 = False
	p21 = True
```
`set`을 사용하는것은 `else`가 없음을 의미한다. 
따라서 `reset`을 활성화하는 조건에 대해서 `reset`의 대상을 `False`로 만든다.

## 선입력 우선
![[Pasted image 20260811111326.png]]

```python
if p0 and not p21:
	p20 = True

if p1 and not p20:
	p21 = True

if p2:
	p20 = False
	p21 = False
```

## 원버튼 온오프
![[Pasted image 20260811134959.png]]LD에서는 다음과 같이 복잡한 구조로 구성해야 하지만 ST를 이용하면 다음과 같이 간단하게 구성할 수 있다.
#### 변수 선언
![[Pasted image 20260811135506.png]]
```pascal
r_trig_0(CLK:=btn0);

IF r_trig_0.Q THEN
	lamp := NOT lamp;

END_IF;
```

## 순차 점등
![[Pasted image 20260811141920.png]]

```pascal
// 1. 버튼 상승 엣지 감지
r_trig_0(CLK := btn0);

// 2. [상태 제어] 버튼을 누를 때만 카운터(c0) 변경 및 0, 1, 2 순환
IF r_trig_0.Q THEN
    c0 := c0 + 1;
    IF c0 > 2 THEN
        c0 := 0; // 0 -> 1 -> 2 -> 0 -> 1 ... 순환
    END_IF;
END_IF;

// 3. [출력 제어] c0 값에 따라 매 스캔마다 램프 상태 결정 (이중 코일 완벽 방지)
lamp0 := (c0 = 0);
lamp1 := (c0 = 1);
lamp2 := (c0 = 2);
```
> **상태 변경과 출력 제어를 완전히 분리하여 작성할 것**
## 타이머
![[Pasted image 20260811172140.png]]

```pascal
ton0(IN:=lamp0, PT:=T#10S);
r_trig_0(CLK:=btn0);

IF  r_trig_0.Q THEN
	lamp0 := TRUE;
END_IF;

IF ton0.Q THEN
	lamp0 := FALSE;
END_IF;
```
`p20`이 Timer를 활성화 하는것은 `ton0`인스턴스를 생성하면서 지정한다.
각 렁마다 if문이 하나씩 들어가는것을 확인할 수 있다.
**펄스신호의 경우 else문이 사용되지 않는다.**

![[Pasted image 20260811172557.png]]


![[Pasted image 20260811173322.png]]
```pascal
ton0(IN:=(btn0 AND btn1), PT:=T#5S);

IF ton0.Q THEN
	lamp0 := TRUE;
ELSE
	lamp0 := FALSE;
END_IF;
```
![[Pasted image 20260812092057.png]]
![[Pasted image 20260812092147.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=flag0, PT:=T#10S);

IF r_trig_0.Q THEN
	flag0 := TRUE;
END_IF;
	
IF ton0.Q THEN
	lamp0 := TRUE;
ELSE
	lamp0 := FALSE;
END_IF;

IF r_trig_1.Q THEN
	flag0 := FALSE;
END_IF;
```

![[Pasted image 20260812092536.png]]![[Pasted image 20260812093607.png]]

```pascal
ton0(IN:=(btn0 AND NOT ton1.Q), PT:=T#1S);
ton1(IN:=(btn0 AND ton0.Q), PT:=T#1S);

IF (btn0 AND NOT ton0.Q) THEN
	lamp0 := TRUE;
ELSE
	lamp0 := FALSE;
END_IF;
		
IF (btn0 AND ton0.Q) THEN
	lamp1 := TRUE;
ELSE
	lamp1 := FALSE;
END_IF;
```
### 플리커 동작
`btn0`가 활성화 되어있을 때만 작동하므로 `btn0`옆의 조건만 확인하면 된다.
1. `btn0`가 활성화 되어 있으면 초기 `ton1.Q`는 `False`이므로 `ton0.Q`가 1초 후 활성화 된다. 
2. `ton0.Q`가 활성화 되므로  조건에 따라 `ton1.Q`도 1초 후 활성화 된다.
3. `ton1.Q`가 활성화되면 `ton0`의 입력조건이 `FALSE`가 되어 비활성화 된다. 
4. `ton0.Q`가 `FALSE`가 되었으므로 `ton1`의 조건도 `FALSE`가 되어 비활성화된다.
5. 이에 따라 `ton0`의 조건이 다시 만족되어 다시 `ton0`가 작동한다.
`ton0`는 `btn0`가 활성화 된 시점에서 1초동안 `FALSE`, 1초동안 `TRUE`를 반복한다. 
이를 이용하여 플리커를 구성한다. 

![[Pasted image 20260812121649.png]]


![[Pasted image 20260812123429.png]]
```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=flag0 AND NOT ton1.Q, PT:=T#2S);
ton1(IN:=flag0 AND ton0.Q, PT:=T#1S);

IF r_trig_0.Q THEN
	flag0 := TRUE;
END_IF;

IF r_trig_1.Q THEN
	flag0 := FALSE;
END_IF;	

IF flag0 THEN
	IF NOT ton0.Q THEN
		lamp0 := TRUE;
		lamp1 := FALSE;
	ELSE
		lamp0 := FALSE;
		lamp1 := TRUE;
		
	END_IF;
END_IF;
```

![[Pasted image 20260812123750.png]]![[Pasted image 20260812124127.png]]

```pascal
ton0(IN:=flag0 AND NOT ton0.Q, PT:=T#5S);
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);

IF r_trig_0.Q THEN
	flag0 := TRUE;
END_IF;

IF r_trig_1.Q THEN
	flag0 := FALSE;
END_IF;

IF flag0 THEN
	IF (ton0.ET > T#0S AND ton0.ET <= T#1S) OR
   	(ton0.ET > T#2S AND ton0.ET <= T#3S) THEN
		lamp0 := TRUE;
	ELSE
		lamp0 := FALSE;
	END_IF;
END_IF;
```
Preset Time이 경과한 Timer를 초기화 하기 위해서 Timer의 IN(Condition)에 TImer의 출력을 넣음으로서 출력이 발생 (TRUE)가 되면 초기화된다. 

![[Pasted image 20260812135738.png]]
![[Pasted image 20260812140154.png]]

```pascal
toff0(IN:=btn0, PT:=T#5S);

IF toff0.Q THEN
	lamp0 := TRUE;
ELSE
	lamp0 := FALSE;
	
END_IF;
```

![[Pasted image 20260812141211.png]]

```pascal
ton0(IN:=flag0, PT:=T#6S);
f_trig_0(CLK:=btn0);

IF f_trig_0.Q THEN
	flag0 := TRUE;
END_IF;

IF flag0 THEN
	
	IF ton0.ET >= T#0S AND ton0.ET < T#2S THEN
		lamp0 := TRUE;
	ELSE
		lamp0 := FALSE;
	END_IF;
		
	IF ton0.ET >= T#0S AND ton0.ET < T#4S THEN
		lamp1 := TRUE;
	ELSE
		lamp1 := FALSE;
	END_IF;
	
	IF ton0.ET >= T#0S AND ton0.ET < T#6S THEN
		lamp2 := TRUE;
	ELSE
		lamp2 := FALSE;
	END_IF;
	
END_IF;

IF ton0.Q THEN
	flag0 := FALSE;
END_IF;
```
### Retentive Timer
![[Pasted image 20260812142517.png]]

```pascal
f_trig_0(CLK:=btn0);

IF f_trig_0.Q THEN
	t0 := DWORD_TO_TIME(TIME_TO_DWORD(t0) - TIME_TO_DWORD(ton0.ET));
	ET := DWORD_TO_TIME(TIME_TO_DWORD(ET) + TIME_TO_DWORD(ton0.ET));
END_IF;
	
IF ton0.Q THEN
	lamp0 := TRUE;
	
ELSE
	lamp0 := FALSE;
	
END_IF;

ton0(IN:=btn0, PT:= t0);
```

>**`ton0`의 실행 조건이 FALSE가 되어 정보가 사라지기전에 데이터를 대입해야 하므로 `ton0`를 가장 아래에 둔다.** 
>**TIME타입의 변수는 비교연산자는 가능하지만 산술연산자는 불가능하므로 DWORD타입으로 바꾸어 연산한다음 다시 TIME타입으로 바꾸어 대입해준다**

### Retrigerble Timer
![[Pasted image 20260812152208.png]]

```pascal
r_trig_0(CLK:=btn0);
ton0(IN:= NOT r_trig_0.Q AND flag0, PT:=T#10S);

IF r_trig_0.Q THEN
	flag0 := TRUE;
END_IF;

IF ton0.Q THEN
	lamp0 := TRUE;
ELSE
	lamp0 := FALSE;
END_IF;
```
`btn0` 의 `rising`을 감지하면 신호가 끊어지도록 하여 타이머를 초기화시키는 방법

![[Pasted image 20260812155936.png]]

```pascal
ton0(IN:=btn0, PT:=T#7S);
f_trig_0(CLK:=btn0);
toff0(IN:=f_trig_0.Q, PT:=T#3S);

//btn0이 TRUE일 때 ton0.ET가 T#3S이상 T#7S미만일 때 또는 떠난걸 감지 (f_trig)하면 3초동안 램프 동작
lamp0 := (btn0 AND (ton0.ET >= T#3S AND ton0.ET < T#7S)) OR (NOT btn0 AND toff0.Q);
```

> **LD의 이중코일 문제와 같이 출력이 무시되는 문제가 발생할 수 있음**
> **따라서 반드시 출력은 한 곳에서만 정해져야 한다.**

### 순차점등

![[Pasted image 20260812170312.png]]

### Elapsed Time을 활용하는 방법
```pascal
r_trig_0(CLK:=btn0);
ton0(IN:=is_running, PT:=T#6S);

IF r_trig_0.Q THEN
	is_running := TRUE;
END_IF;

IF ton0.Q THEN
	is_running := FALSE;
END_IF;

lamp0 := is_running AND (ton0.ET >= T#0S AND ton0.ET < T#2S);
lamp1 := is_running AND (ton0.ET >= T#2S AND ton0.ET < T#4S);
lamp2 := is_running AND (ton0.ET >= T#4S AND ton0.ET < T#6S); 
```

### counter를 활용하는 법

```pascal
r_trig_0(CLK:=btn0);

IF r_trig_0.Q AND c0 = 0 THEN
	c0 := 1;
ELSIF ton0.Q AND c0 = 1 THEN
	c0 := 2;
ELSIF ton0.Q AND c0 = 2 THEN
	c0 := 3;
ELSIF ton0.Q AND c0 = 3 THEN
	c0 := 0; // IDLE
END_IF;
	
ton0(IN:=(c0>=1) AND NOT ton0.Q, PT:=T#2S);

lamp0 := c0 = 1;
lamp1 := c0 = 2;
lamp2 := c0 = 3;
```

![[Pasted image 20260812173416.png]]![[Pasted image 20260814093238.png]]


```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=mode=1, PT:=T#2S);
ton1(IN:=ton0.Q, PT:=T#2S);
ton2(IN:=mode=2, PT:=T#2S);
ton3(IN:=ton2.Q, PT:=T#2S);

IF r_trig_0.Q AND (mode = 0) THEN
    mode := 1; // 정방향 점등
ELSIF r_trig_1.Q AND (mode = 1) THEN
    mode := 2; // 정방향 소등
ELSIF ton3.Q AND (mode = 2) THEN
    mode := 0; // IDLE
END_IF;

CASE mode OF
    0:
        lamp0 := FALSE;
        lamp1 := FALSE;
        lamp2 := FALSE;
    1:
        lamp0 := TRUE;
        lamp1 := ton0.Q;
        lamp2 := ton1.Q;
    2:
        lamp0 := FALSE;
        lamp1 := NOT ton2.Q;
        lamp2 := NOT ton3.Q;
END_CASE;
```

> FSM 기반 프로그래밍으로 State, Input, Transition, Action을 나누어 생각할 것

![[Pasted image 20260814105123.png]]![[Pasted image 20260814105540.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=(mode=1) AND NOT ton2.Q, PT:=T#2S);
ton1(IN:=ton0.Q, PT:=T#2S);
ton2(IN:=ton1.Q, PT:=T#2S);

// STATE, INPUT, ACTION
IF r_trig_0.Q AND (mode = 0) THEN
	mode := 1; // RUNNING
ELSIF r_trig_1.Q AND (mode = 1) THEN
	mode := 0; // IDLE
END_IF;

// ACTION, 
CASE mode OF
	0: 
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
		
	1: 
		lamp0 := NOT ton0.Q;
		lamp1 := ton0.Q AND NOT ton1.Q;
		lamp2 := ton1.Q AND NOT ton2.Q;
END_CASE;
```

![[Pasted image 20260814111543.png]]![[Pasted image 20260814112200.png]]

```pascal
r_trig_0(CLK:=btn0);
ton0(IN:= is_running AND NOT ton2.Q, PT:=T#2S);
ton1(IN:= ton0.Q, PT:=T#2S);
ton2(IN:= ton1.Q, PT:=T#2S);
ton3(IN:= ton2.Q, PT:=T#2S);


// STATE, INPUT, TRANSITION
IF r_trig_0.Q AND NOT is_running THEN
	is_running := TRUE;
ELSIF r_trig_0.Q AND is_running THEN
	is_running := FALSE;
END_IF;

//ACTION
IF is_running THEN
	lamp0 := NOT ton0.Q;
	lamp1 := (ton0.Q AND NOT ton1.Q) OR (ton2.Q AND NOT ton3.Q);
	lamp2 := ton1.Q AND NOT ton2.Q;
ELSE
	lamp0 := FALSE;
	lamp1 := FALSE;
	lamp2 := FALSE;
END_IF;
```
> FLAG를 사용한 FSM Based Programming.
> 2가지 경우 뿐이므로 IF ELSE로 표현이 가능하였다.


![[Pasted image 20260814114001.png]]

```pascal
r_trig_0(CLK:=btn0);
ton0(IN:=is_running AND NOT ton0.Q, PT:=T#8S);

//STATE, INPUT, TRANSITION
IF r_trig_0.Q AND NOT is_running THEN
	is_running := TRUE;
ELSIF r_trig_0.Q AND is_running THEN
	is_running := FALSE;
END_IF;

IF is_running THEN
	lamp0 := ton0.ET >= T#0S AND ton0.ET < T#2S;
	lamp1 := (ton0.ET >= T#2S AND ton0.ET < T#4S) OR (ton0.ET >= T#6S AND ton0.ET < T#8S);
	lamp2 := ton0.ET >= T#2S AND ton0.ET < T#8S ;
	
ELSE
	lamp0 := FALSE;
	lamp1 := FALSE;
	lamp2 := FALSE;
		
END_IF;
```

>비교 연산의 결과를 바로 대입함으로써 간결한 코드를 구성 가능
>역시 플래그를 사용하였기 때문에 IF ELSE로 모든 케이스를 정리


![[Pasted image 20260814115236.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=mode=1 AND lamp0, PT:=T#2S);
ton1(IN:=mode=1 AND lamp1, PT:=T#2S);
ton2(IN:=mode=2 AND NOT lamp2, PT:=T#2S);
ton3(IN:=mode=2 AND NOT lamp1, PT:=T#2S);

IF mode = 0 AND r_trig_0.Q THEN
	mode := 1;
ELSIF mode = 1 AND r_trig_1.Q THEN
	mode := 2;
ELSIF mode = 2 AND ton3.Q THEN
	mode := 0;
END_IF;

CASE mode OF 

	0: 
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
	
	1:
		lamp0 := TRUE;
		lamp1 := ton0.Q;
		lamp2 := ton1.Q;
		
	2: 
		lamp0 := NOT ton3.Q;
		lamp1 := NOT ton2.Q;
		lamp2 := FALSE;
		
END_CASE;
```







![[Pasted image 20260817144613.png]]

![[Pasted image 20260817150953.png]]
![[Pasted image 20260817150959.png]]
![[Pasted image 20260817151011.png]]

```pascal
// State, Input, Transition, Action
// IDLE, SPEAK0~3, SCORING: 6 STATES
r_trig_0(CLK:=btn0); // RESET
r_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
r_trig_3(CLK:=btn3);
r_trig_4(CLK:=btn4);
r_trig_5(CLK:=btn5);
f_trig_0(CLK:=lamp2 AND mode = 5);
ton1(IN:=mode=1, PT:=T#2S);
ton2(IN:=mode=2, PT:=T#2S);
ton3(IN:=mode=3, PT:=T#2S);
ton4(IN:=mode=4, PT:=T#2S);
ton5(IN:=mode=5 AND NOT ton5.Q, PT:=T#2S);

// TRANSITION, INPUT, STATE
IF mode = 0 AND r_trig_1.Q THEN
	mode := 1; // SPEAK0
ELSIF mode = 0 AND r_trig_2.Q THEN
	mode := 2; //SPEAK1
ELSIF mode = 0 AND r_trig_3.Q THEN
	mode := 3; //SPEAK2
ELSIF mode = 0 AND r_trig_4.Q THEN
	mode := 4; //SPEAK3
ELSIF r_trig_5.Q THEN
	mode := 5; // SCORING
ELSIF (mode >= 1 AND r_trig_0.Q) OR counter = 3 THEN
	mode := 0; //IDLE 

END_IF;

// ACTION
CASE mode OF 
	0:
		counter := 0;
		lamp1 := FALSE;
		lamp2 := FALSE;
		lamp3 := FALSE;
		lamp4 := FALSE;
		lamp5 := FALSE;
	
	1:
		lamp1 := NOT ton1.Q;
		
	2:
		lamp2 := NOT ton2.Q;
		
	3:
		lamp3 := NOT ton3.Q;
		
	4:
		lamp4 := NOT ton4.Q;

	5:
		IF f_trig_0.Q THEN
			counter := counter + 1;
		END_IF;
		
		lamp1 := ton5.ET < T#1S;
		lamp2 := ton5.ET >= T#1S;
		lamp3 := ton5.ET < T#1S;
		lamp4 := ton5.ET >= T#1S;
		lamp5:= TRUE;
END_CASE;
```

![[Pasted image 20260817165945.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
ton0(IN:=mode=1 AND NOT ton0.Q, PT:=T#2S);
ton1(IN:=mode=2 AND NOT ton1.Q, PT:=T#2S);

IF r_trig_0.Q THEN
	mode := 1;
ELSIF r_trig_1.Q THEN
	mode := 2;
ELSIF mode >= 1 AND r_trig_2.Q THEN
	mode := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
	1:
		lamp0 := ton0.ET < T#1S;
		lamp1 := FALSE;
	2:
		lamp0 := FALSE;
		lamp1 := ton1.ET < T#1S;
END_CASE;
```

![[Pasted image 20260817183119.png]]

```pascal
r_trig_0(CLK:=btn0);

IF r_trig_0.Q THEN
	counter := counter + 1;
END_IF;

lamp0 := counter >= 4;
```

![[Pasted image 20260817183533.png]]

```pascal
r_trig_0(CLK:=btn0);
toff0(IN:=r_trig_0.Q, PT:=T#2S);

IF r_trig_0.Q THEN
	counter := counter + 1;
END_IF;

lamp0 := toff0.Q;
lamp1 := counter >= 2;
```

![[Pasted image 20260817190228.png]]![[Pasted image 20260817191925.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=lamp0);
ton0(IN:=is_running AND NOT ton0.Q, PT:=T#2S);

IF r_trig_0.Q AND is_running = FALSE THEN
	is_running := TRUE;
ELSIF r_trig_0.Q AND is_running = TRUE THEN
	is_running := FALSE;
	counter := 0;
END_IF;

IF r_trig_1.Q THEN
	counter := counter + 1;
END_IF;

lamp0 := is_running AND ton0.ET < T#1S;

r_trig_1(CLK:=lamp0);

lamp1 := is_running AND (ton0.ET < T#1S) AND (counter MOD 3 = 0);

```

![[Pasted image 20260817193825.png]]

![[Pasted image 20260817193817.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=lamp0);

ton0(IN:=is_running AND NOT ton0.Q, PT:=T#2S);
toff0(IN:=r_trig_0.Q, PT:=T#2S);

IF r_trig_0.Q AND is_running = FALSE THEN
	is_running := TRUE;
ELSIF r_trig_0.Q AND is_running = TRUE THEN
	is_running := FALSE;
	counter := 0;
END_IF;

lamp0 := is_running AND ton0.ET < T#1S;

r_trig_1(CLK:=lamp0);

IF r_trig_1.Q THEN
	counter := counter + 1;
END_IF;

lamp1 := is_running  AND (counter MOD 3 = 0);
```

![[Pasted image 20260817195152.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
ton0(IN:= mode=2 AND NOT ton0.Q, PT:=T#2S);

IF r_trig_0.Q AND mode = 0 THEN
	mode := 1;
ELSIF r_trig_1.Q AND mode = 1 THEN
	mode := 2;
ELSIF r_trig_2.Q AND mode = 2 THEN
	mode := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		
	1:
		lamp0 := TRUE;
		lamp1 := FALSE;
	
	2:
		lamp0 := FALSE;
		lamp1 := ton0.ET < T#1S;
END_CASE;
```

![[Pasted image 20260817200157.png]]

```pascal
r_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);

ton0(IN:= mode=1 AND NOT ton0.Q, PT:=T#2S);
ton1(IN:= mode=2 AND NOT ton1.Q, PT:=T#3S);

IF r_trig_0.Q THEN
	mode := 1;
ELSIF r_trig_1.Q THEN
	mode := 2;
ELSIF r_trig_2.Q THEN
	mode := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		
	
	1:
		lamp0 := ton0.ET < T#1S;
		
		
	2:
		lamp0 := ton1.ET < T#2S;
		
END_CASE;
```

![[Pasted image 20260817201039.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);

ton0(IN:=(mode=1) AND (NOT ton0.Q), PT:=T#2S);
ton1(IN:=(mode=2) AND (NOT ton1.Q), PT:=T#2S);

// STATE, INPUT, TRANSITION
IF f_trig_0.Q AND mode = 0 THEN
	mode := 1;
ELSIF r_trig_0.Q AND mode = 1 THEN
	mode := 2;
ELSIF r_trig_1.Q AND mode = 2 THEN
	mode := 0;
END_IF;

// ACTION
CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
	
	1:
		lamp0 := ton0.ET < T#1S;
		lamp1 := ton0.ET >= T#1S;
		lamp2 := FALSE;
	
	2:
		lamp0 := FALSE;
		lamp1 := ton1.ET >= T#1S;
		lamp2 := ton1.ET < T#1S;

END_CASE;
```

![[Pasted image 20260818093034.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);

IF r_trig_0.Q THEN
	counter := counter + 1;
END_IF;

IF r_trig_1.Q THEN
	counter := 0;
END_IF;

lamp0 := counter >= 2;
lamp1 := counter >= 3;
lamp2 := counter >= 4;
```

![[Pasted image 20260818093623.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=lamp0, PT:=T#1S);
ton1(IN:=lamp1 AND NOT ton1.Q, PT:=T#2S);

IF f_trig_0.Q THEN
	counter := counter + 1;
END_IF;

IF r_trig_1.Q THEN
	counter := 0;
END_IF;

lamp0 := counter >= 3;
lamp1 := lamp0 AND ton0.Q;
lamp2 := ton1.ET >= T#1S;
```

![[Pasted image 20260818094125.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
ton0(IN:=is_running AND is_switching AND NOT ton0.Q, PT:=T#4S);


IF r_trig_0.Q AND NOT is_running THEN
	is_running := TRUE;
ELSIF r_trig_0.Q AND is_running THEN
	is_running := FALSE;	
	is_switching := FALSE;
END_IF;

IF r_trig_1.Q AND NOT is_switching THEN
	is_switching := TRUE;
END_IF;

	

IF is_running AND NOT is_switching THEN
	lamp0 := TRUE;
	lamp1 := FALSE;
ELSIF is_running AND is_switching THEN
	lamp0 := ton0.ET < T#2S;
	lamp1 := ton0.ET >= T#2S;
ELSIF NOT is_running AND NOT is_switching THEN
	lamp0 := FALSE;
	lamp1 := FALSE;
END_IF;
```
![[Pasted image 20260818095239.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:=counter_flag AND NOT ton0.Q, PT:=T#2S);
ton1(IN:=counter_flag, PT:=T#2S);

IF f_trig_0.Q THEN
	counter := counter + 1;
ELSIF f_trig_1.Q THEN
	counter := 0;
END_IF;

counter_flag := counter >= 3;

lamp0 := counter_flag AND ton0.ET < T#1S;
lamp1 := counter_flag  AND ton0.ET >= T#1S;
lamp2 := counter_flag  AND ton0.ET < T#1S AND ton1.Q;
```

![[Pasted image 20260818100435.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:=mode=1, PT:=T#4S);
ton1(IN:=mode=2, PT:=T#4S);

IF f_trig_0.Q THEN
	counter := counter + 1;	
END_IF;
counter_flag := counter >= 2;

IF counter_flag AND mode = 0 THEN
	mode := 1;
	
ELSIF counter_flag AND r_trig_1.Q AND mode = 1 THEN
	mode := 2;
	
ELSIF ton1.Q AND mode = 2 THEN
	mode := 0; // IDLE
	counter := 0;

END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
		
	1:
		lamp0 := ton0.ET < T#2S;
		lamp1 := ton0.ET >= T#2S AND NOT ton0.Q;
		lamp2 := ton0.Q;
		
	2:
		lamp0 := ton1.ET >= T#2S;
		lamp1 := ton1.ET < T#2S;
		lamp2 := FALSE;
	
END_CASE;
```

![[Pasted image 20260818111440.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:=btn0, PT:=T#3S);
ton1(IN:=btn1, PT:=T#3S);
ton2(IN:=is_running, PT:=T#4S);

IF ton0.Q AND NOT is_running THEN
	is_running := TRUE;

ELSIF ton1.Q AND is_running THEN
	is_running := FALSE;

END_IF;

IF is_running THEN
	lamp0 := TRUE;
	lamp1 := ton2.ET >= T#2S;
	lamp2 := ton2.Q;

ELSE 
	lamp0 := FALSE;
	lamp1 := FALSE;
	lamp2 := FALSE;
	
END_IF;
```

![[Pasted image 20260818114649.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:=is_running, PT:=T#13S);

IF r_trig_0.Q THEN
	is_running := NOT is_running;
	
END_IF;

IF is_running THEN
	lamp0 := (ton0.ET >= T#0S AND ton0.ET < T#3S) OR (ton0.ET >= T#10S AND ton0.ET < T#13S);
	lamp1 := (ton0.ET >= T#3S AND ton0.ET < T#6S) OR (ton0.ET >= T#7S AND ton0.ET < T#10S);
ELSE
	lamp0 := FALSE;
	lamp1 := FALSE;
END_IF;
```

![[Pasted image 20260818115734.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:= (mode=1), PT:=T#4S);
ton1(IN:=mode=2, PT:=T#4S);
IF r_trig_0.Q THEN
	counter := counter + 1;
END_IF;

counter_flag := counter >= 2;

IF mode = 0 AND counter_flag THEN
	mode := 1;
ELSIF mode = 1 AND r_trig_1.Q THEN
	mode := 2;
ELSIF mode = 2 AND ton1.Q THEN
	mode := 0;
	counter := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
	
	1: 
		lamp0 := TRUE;
		lamp1 := ton0.ET >= T#2S;
		lamp2 := ton0.Q;
	
	2: 
		lamp0 := NOT ton1.Q;
		lamp1 := NOT (ton1.ET >= T#2S);
		lamp2 := FALSE;
		
END_CASE;
```

![[Pasted image 20260818122241.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:= (mode=1), PT:=T#4S);
ton1(IN:=mode=2, PT:=T#4S);

IF r_trig_0.Q THEN
	counter0 := counter0 + 1;
ELSIF r_trig_1.Q THEN
	counter1 := counter1 + 1;
END_IF;

lamp0 := counter0 >= 1 AND counter1 < 3;
lamp1 := counter0 >= 2 AND counter1 < 2;
lamp2 := counter0 >= 3 AND counter1 < 1;

IF counter0 >= 3 AND counter1 >= 3 THEN
	counter0 := 0;
	counter1 := 0;
END_IF;
```

![[Pasted image 20260818123825.png]]
```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
ton0(IN:= counter_flag AND is_running, PT:=T#7S);
ton1(IN:=lamp2, PT:=T#5S);

IF r_trig_0.Q AND NOT is_running THEN 
	is_running := TRUE;
ELSIF r_trig_1.Q THEN
	counter0 := counter0 + 1;
END_IF;

counter_flag := counter0 >= 3;

IF counter_flag AND is_running THEN
	lamp0 := ton0.ET < T#5S;
	lamp1 := ton0.ET >= T#1S AND ton0.ET < T#6S;
	lamp2 := ton0.ET >= T#2S AND ton0.ET < T#7S;
	
	IF ton0.Q THEN
		counter0 := 0;
		is_running := FALSE;
	END_IF;
	
ELSE
	lamp0 := FALSE;
	lamp1 := FALSE;
	lamp2 := FALSE;
END_IF;
```

![[Pasted image 20260818132721.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
ton0(IN:= mode=1, PT:=T#7S);
ton1(IN:= mode=2, PT:=T#5S);

IF mode = 0 AND r_trig_0.Q THEN
	mode := 1;

ELSIF mode = 0 AND r_trig_1.Q THEN
	mode := 2;
	
ELSIF mode = 1 AND ton0.Q THEN
	mode := 0;
	
ELSIF mode = 2 AND ton1.Q THEN
	mode := 0;

ELSIF r_trig_2.Q THEN
	mode := 0;
	
END_IF;

CASE mode OF
	
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
	
	1: 
		lamp0 := (ton0.ET >= T#0S AND ton0.ET < T#5S);
		lamp1 := (ton0.ET >= T#1S AND ton0.ET < T#6S);
		lamp2 := (ton0.ET >= T#2S AND ton0.ET < T#7S);
	
	
	2:
		lamp0 := (ton1.ET >= T#2S AND ton1.ET < T#7S);
		lamp1 := (ton1.ET >= T#1S AND ton1.ET < T#6S);
		lamp2 := (ton1.ET >= T#0S AND ton1.ET < T#5S);
	
END_CASE;
```

![[Pasted image 20260818134221.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
ton0(IN:=is_running AND NOT ton0.Q, PT:=T#2S);


IF r_trig_0.Q THEN
	is_running := NOT is_running;
END_IF;

IF is_running THEN
	lamp0 := ton0.ET < T#1S;
	
END_IF;

lamp1 := btn0;
```

![[Pasted image 20260818135546.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
ton0(IN:=is_running AND NOT ton0.Q, PT:=T#2S);
ton1(IN:=is_running, PT:=T#2S);


IF r_trig_0.Q THEN
	is_running := NOT is_running;
END_IF;

IF is_running THEN
	lamp0 := ton0.ET < T#1S;
	lamp1 := ton0.ET >= T#1S;
	lamp2 := ton0.ET < T#1S AND ton1.Q;

ELSE
	lamp0 := FALSE;
	lamp1 := FALSE;
	lamp2 := FALSE;
	
END_IF;
```

![[Pasted image 20260818140230.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
r_trig_3(CLK:=lamp1);
f_trig_3(CLK:=lamp1);
ton0(IN:=mode=1, PT:=T#1S);
ton1(IN:=mode=2 AND NOT ton1.Q, PT:=T#2S);

IF f_trig_0.Q THEN
	lamp0 := TRUE;
END_IF;

IF r_trig_1.Q AND lamp0 AND mode = 0 THEN
	mode := 1;
ELSIF r_trig_1.Q AND lamp0 AND mode = 1 THEN
	mode := 2;
ELSIF r_trig_2.Q OR counter0 = 4 THEN
	mode := 0;
	lamp0 := FALSE;
	
END_IF;

IF mode=2 AND f_trig_3.Q THEN
	counter0 := counter0 + 1;
END_IF;

CASE mode OF 
	0:
		lamp1 := FALSE;
		lamp2 := FALSE;
		counter0 := 0;
	
	1:
		lamp1 := TRUE;
		lamp2 := ton0.Q;
		
	2:
		lamp1 := ton1.ET >= T#1S;
		lamp2 := ton1.ET >= T#1S;
END_CASE;
```

![[Pasted image 20260818143815.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
r_trig_3(CLK:=lamp1);
f_trig_3(CLK:=lamp1);

IF r_trig_0.Q THEN
	counter0 := counter0 + 1;
ELSIF r_trig_1.Q THEN
	counter0 := counter0 - 1;
ELSIF r_trig_2.Q THEN
	counter0 := 0;
END_IF;

lamp0 := counter0 = 3;

```
![[Pasted image 20260818144930.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
ton0(IN:=mode=1 AND NOT ton0.Q, PT:=T#2S);
ton1(IN:=mode=2 AND NOT ton1.Q, PT:=T#2S);

IF mode=0 AND r_trig_0.Q THEN
	mode := 1;
ELSIF mode = 1 AND r_trig_1.Q THEN
	mode := 2;
ELSIF mode = 2 AND r_trig_1.Q THEN
	mode := 1;
ELSIF mode >= 1 AND r_trig_2.Q THEN
	mode := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := TRUE;
		lamp1 := FALSE;
		lamp2 := FALSE;
		lamp3 := FALSE;
		lamp4 := FALSE;
		lamp5 := FALSE;
	1:
		lamp0 := FALSE;
		lamp1 := TRUE;
		lamp2 := TRUE;
		lamp3 := FALSE;
		lamp4 := ton0.ET < T#1S;
		lamp5 := FALSE;
		
	2:
		lamp0 := FALSE;
		lamp1 := TRUE;
		lamp2 := FALSE;
		lamp3 := TRUE;
		lamp4 := FALSE;
		lamp5 := ton1.ET < T#1S;
END_CASE;
```

![[Pasted image 20260818150920.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
ton0(IN:=mode=1 AND NOT ton0.Q, PT:=T#3S);
ton1(IN:=mode=2 AND NOT ton1.Q, PT:=T#3S);
ton2(IN:=NOT ton2.Q, PT:=T#3S);

lamp7 := ton2.ET < T#2S;
IF mode=0 AND f_trig_0.Q THEN
	mode := 1;
ELSIF mode = 1 AND f_trig_0.Q THEN
	mode := 2;
ELSIF mode = 2 AND f_trig_0.Q THEN
	mode := 1;
ELSIF mode >= 1 AND r_trig_1.Q THEN
	mode := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
	
	1:
		lamp0 := (ton0.ET >= T#0S AND ton0.ET < T#1S);
		lamp1 := (ton0.ET >= T#1S AND ton0.ET < T#2S);
		lamp2 := (ton0.ET >= T#2S AND ton0.ET < T#3S);
		
	2:
		lamp0 := (ton1.ET >= T#2S AND ton1.ET < T#3S);
		lamp1 := (ton1.ET >= T#1S AND ton1.ET < T#2S);
		lamp2 := (ton1.ET >= T#0S AND ton1.ET < T#1S);
	
END_CASE;
```

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);

ton0(IN:=NOT ton0.Q, PT:=T#1S);
r_trig_3(CLK:=ton0.Q);
ton2(IN:=NOT ton2.Q, PT:=T#3S);

lamp7 := ton2.ET < T#2S;

IF r_trig_3.Q THEN
	counter := (counter + 1) MOD 3;
END_IF;

IF mode = 0 AND f_trig_0.Q THEN
	mode := 1;
ELSIF mode = 1 AND f_trig_0.Q THEN
	mode := 2;
ELSIF mode = 2 AND f_trig_0.Q THEN
	mode := 1;
ELSIF mode >= 1 AND r_trig_1.Q THEN
	mode := 0;
END_IF;

CASE mode OF
	0:
		lamp0 := FALSE;
		lamp1 := FALSE;
		lamp2 := FALSE;
	
	1:
		lamp0 := counter = 0;
		lamp1 := counter = 1;
		lamp2 := counter = 2;
		
	2:
		lamp0 := counter = 2;
		lamp1 := counter = 1;
		lamp2 := counter = 0;
END_CASE;
```

![[Pasted image 20260818164915.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
r_trig_3(CLK:=btn3);
f_trig_3(CLK:=btn3);

ton0(IN:= NOT ton0.Q, PT:= T#2S);
lamp0 := ton0.ET <= T#1S;

IF r_trig_0.Q AND NOT is_running THEN
	is_running := TRUE;
	
ELSIF r_trig_0.Q AND is_running THEN
	counter := (counter + 1) MOD 4;

ELSIF r_trig_1.Q AND is_running THEN
	is_running := FALSE;
	counter := 0;
END_IF;

lamp1 := is_running AND counter = 0;
lamp2 := is_running AND (counter = 1 OR counter = 3);
lamp3 := is_running AND counter = 2;
```

![[Pasted image 20260818171821.png]]

```pascal
r_trig_0(CLK:=btn0);
f_trig_0(CLK:=btn0);
r_trig_1(CLK:=btn1);
f_trig_1(CLK:=btn1);
r_trig_2(CLK:=btn2);
f_trig_2(CLK:=btn2);
r_trig_3(CLK:=btn3);
f_trig_3(CLK:=btn3);

ton0(IN:= NOT ton0.Q, PT:= T#5S);
lamp0 := (ton0.ET <= T#1S) OR (ton0.ET > T#2S AND ton0.ET <= T#4S);

ton1(IN:=is_running AND NOT ton4.Q, PT:=T#2S);
ton2(IN:=ton1.Q, PT:=T#2S);
ton3(IN:=ton2.Q, PT:=T#2S);
ton4(IN:=ton3.Q, PT:=T#2S);

IF f_trig_0.Q AND NOT is_running THEN
	is_running := TRUE;
ELSIF r_trig_1.Q AND is_running THEN
	is_running := FALSE;
END_IF;

IF is_running THEN
	lamp1 := ton2.Q AND NOT ton3.Q;
	lamp2 := (ton1.Q AND NOT ton2.Q) OR (ton3.Q AND NOT ton4.Q);
	lamp3 := NOT ton1.Q;

ELSE
	lamp1 := FALSE;
	lamp2 := FALSE;
	lamp3 := FALSE;

END_IF;
```

