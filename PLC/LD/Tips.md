- Rising의 경우와 Falling의 경우를 분리해서 Counter로 처리하면 구현하기 쉬워짐
- 스캔과 인터프리터 방식은 비슷하므로 래더를 작성할 때 인터프리터가 코드의 그 라인을 조건문으로 읽는다고 생각하면 편하다.
- 입력과 출력의 관계를 살펴보자
![[Pasted image 20260806103630.png]]
- 다음과 같은 래더는 파이썬으로 생각하면 다음과 같다
```python
if M0 and not(M1):
	P20 = True
	
if M0 and not(M2):
	P21 = True
	
if M0 and not(M3):
	P22 = True
```