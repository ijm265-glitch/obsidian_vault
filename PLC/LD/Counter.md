## 용어 및 변수 정의

- C : 카운터 번호 및 메모리 주소 (예: C0000)
- PV : 설정값 (Preset Value, 카운트 목표 숫자)
- CV : 현재값 (Current Value, 현재 누적된 숫자)
- CU : 카운트 업 입력 신호 (Off에서 On으로 변할 때 동작)
- CD : 카운트 다운 입력 신호 (Off에서 On으로 변할 때 동작)
- R : 리셋 신호 (Reset)
    

## 카운터 종류별 구문 및 동작

### 1. CTU (Up Counter)

- 구문 : CTU C PV
    
- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호
    
- 동작 방식 : 입력 신호가 들어올 때마다 CV가 1씩 증가
    
- 출력 조건 : CV가 PV 이상(CV >= PV)이 되면 카운터 출력 접점이 On
    
- 특징 : R(리셋) 신호가 들어오면 CV는 0으로 초기화됨
    

### 2. CTD (Down Counter)

- 구문 : CTD C PV
    
- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호
    
- 동작 방식 : 입력 신호가 들어올 때마다 CV가 1씩 감소
    
- 출력 조건 : CV가 0이 되면 카운터 출력 접점이 On
    
- 특징 : R(리셋) 신호가 들어오면 CV가 0이 아닌 설정값 PV로 채워짐
    

### 3. CTUD (Up-Down Counter)

- 구문 : CTUD C CU_신호 CD_신호 PV
    
- 입력 조건 : CU 또는 CD 신호의 Low에서 High 상승 Edge
    
- 동작 방식 : CU 신호가 오면 CV가 1 증가, CD 신호가 오면 CV가 1 감소
    
- 출력 조건
    - Up 출력 : CV가 PV 이상(CV >= PV)일 때 On

### 4. CTR (Ring Counter)

- 구문 : CTR C PV
    
- 입력 조건 : Low에서 High로 변하는 상승 Edge 신호
    
- 동작 방식 : 0부터 시작해 PV까지 1씩 증가하다가, PV에 도달한 후 다음 신호가 들어오면 다시 0으로 돌아가 순환
    
- 특징 : 설정한 범위(0~PV) 내에서 숫자가 무한히 돌며 카운팅됨