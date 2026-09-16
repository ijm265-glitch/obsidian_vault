![[Pasted image 20260912212713.png]]

- **External Types (외부 타입)**
    - C++ 모듈이나 안전(Safety) 모듈 등 외부에서 정의된 구조체나 데이터 타입을 PLC로 가져와 쓸 때 등록되는 곳입니다. 일반적인 PLC 단독 개발 시에는 비어 있는 상태로 유지됩니다.
        
- **References (참조 라이브러리)**
    - C#의 NuGet이나 C++의 헤더 파일 링크처럼, PLC에서 기본으로 사용하는 내장 라이브러리 목록입니다.
    - `Tc2_Standard`: 타이머(`TON`, `TOF`), 카운터(`CTU`), 문자열 처리 등 PLC 표준 기본 함수 포함.
    - `Tc2_System`: 메시지 로깅(`ADSLOGSTR`), ADS 통신 제어 등 시스템 관련 함수 포함.
    - `Tc3_Module`: TwinCAT 3 코어 인터페이스 연동 라이브러리.
        
- **DUTs (Data Unit Types - 사용자 정의 데이터 타입)**
    - C/C++의 `struct`나 `enum`처럼 사용자가 직접 정의한 복합 데이터 타입을 보관하는 곳입니다.
    - 예: 실린더 제어용 구조체 `ST_Cylinder(bForward, bBackward, bSensor...)`나 상태 머신용 열거형 `E_State(IDLE, RUN, ERROR...)`를 만들 때 사용합니다.
        
- **[[GVLs]] (Global Variable Lists - 전역 변수 목록)**
    - 프로젝트 내의 모든 프로그램(POU)과 외부 C# HMI, 물리 I/O가 공용으로 접근할 수 있는 **전역 변수**를 선언하는 곳입니다.
    - C# HMI 연동 변수나 비상정지 플래그 등을 주로 여기에 선언합니다.
        
- **POUs (Program Organization Units - 프로그램 조직 단위)**
    - 실제 제어 알고리즘 코드를 작성하는 핵심 폴더입니다. 3가지 종류가 들어갑니다.
        - **PRG (Program):** 독자적인 메모리를 가지며 태스크에 의해 주기적으로 실행되는 메인 프로그램 (현재 들어있는 `MAIN (PRG)`이 C/C++의 `main()` 역할).
        - **FB (Function Block):** C++의 클래스/인스턴스처럼 내부 상태(메모리)를 유지하며 호출되는 재사용 블록 (예: 실린더 제어 블록, 모터 제어 블록).
        - **FUN (Function):** 내부 상태 메모리 없이 입력값을 받아 결과만 반환하는 순수 함수 (예: 수학 수식 계산).
            
- **VISUs (Visualizations - 내장 작화 화면)**
    - 별도의 외부 C# HMI 프로그램 없이, TwinCAT 내부에서 버튼이나 램프, 게이지를 배치해 간단히 시뮬레이션할 수 있는 내장 모니터링 화면을 만드는 폴더입니다.
        
- **PlcTask (PlcTask) $\to$ MAIN**
    - 실시간 태스크와 실제 코드가 연결된 곳
    - "매 주기(기본 10ms)마다 `MAIN` 프로그램을 한 번씩 실행하라"고 등록되어 있는 실행 매핑 항목입니다.
        
- **Program1 Instance**
    - 작성된 `Program1 Project`가 TwinCAT 런타임 메모리에 실제 하나의 실행 인스턴스(포트 `851`)로 올라가도록 연결해 주는 런타임 등록 엔티티입니다.
