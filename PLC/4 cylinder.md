![[Pasted image 20260914141303.png]]1 : 복동
2, 3, 4 : 단동

| Cylinder | bSens_home | bSens_work | Push | Pull | Type   |
| -------- | ---------- | ---------- | ---- | ---- | ------ |
| 1        | P0         | P1         | P20  | P21  | Double |
| 2        | P2         | P3         | P22  |      | Single |
| 3        | P4         | P5         | P23  |      | Single |
| 4        | P6         | P7         | P24  |      | Single |



FSM 기반으로 프로그래밍 할 것 
State, Input, Transition, Action 
![[Pasted image 20260914151038.png]]

여기서 State는 10
Input은 M1 (실린더1의 sens_work)
Transition은 State 10에서 20으로
Action은 state 10에서 p20을 작동시키는것