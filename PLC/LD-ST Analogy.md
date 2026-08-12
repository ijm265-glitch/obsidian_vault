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
r_trig_0(CLK:=btn0);

IF r_trig_0.Q THEN
	
	CASE c0 OF
		0 : lamp0 := TRUE; lamp1 := FALSE; lamp2 := FALSE;
		1 : lamp0 := FALSE; lamp1 := TRUE; lamp2 := FALSE;
		2 : lamp0 := FALSE; lamp1 := FALSE; lamp2 := TRUE;
		
	ELSE
		lamp0 := FALSE; lamp1 := FALSE; lamp2 := FALSE;
		
	END_CASE;

	c0 := c0 + 1;
	
END_IF;
```

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