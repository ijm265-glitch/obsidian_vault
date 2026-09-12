하드웨어 I/O 매핑 변수는 GVL에 선언하는것이 표준

```
VAR_GLOBAL
	bSens_home AT %I* : BOOL; // %I* : 하드웨어 입력(Input) 단자와 자동 링크할 수 있는 변수
	bCyl_fwd AT %Q*: BOOL;
END_VAR
```

![[Pasted image 20260912215558.png]]

이후 입력모듈에 들어가서 입력채널을 전부 선택 후 Change Multi Link![[Pasted image 20260912215709.png]]all types를 선택하고 앞서 선언한 GVs를 선택해서 링크