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
1. 기본적으로 False로 초기화 되어있으며 입력디바이스(p0, 01)는 임의로 변경 가능한 값이다.
2. 크게 `while True:`로 감싸져 있다고 생각하자.
3. 래더의 한 렁은 독립적인 조건문 `if, else`과 같다. 
4. 병렬 연결은 조건문의 or에 해당하며, 직렬 연결은 조건문의 and에 해당한다.


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
