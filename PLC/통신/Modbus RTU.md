### Part 1: 프로토콜 기초 & 프레임 구조

- [[Modbus의 역사와 계층 구조]] (OSI 7계층 관점에서의 Modbus RTU vs ASCII vs TCP)

- 물리 계층의 이해 (RS-485 반이중(Half-Duplex) 통신 및 트랜시버 방향 제어)
    
- [[4대 메모리 맵]](Coil, Discrete Input, Input Register, Holding Register)과 0-based vs 1-based 주소 표기법
    
- T3.5, T1.5 시간 간격(Inter-frame delay)과 프레임 동기화 원리
    

### Part 2: 패킷 심화 분석 및 에러 처리

- 주요 Function Code 분석:
    
    - 단일 읽기/쓰기 (`0x03`, `0x04`, `0x05`, `0x06`)
        
    - 다중 읽기/쓰기 (`0x0F`, `0x10`)
        
- 예외(Exception) 응답 구조와 에러 코드 (`0x01`: 불법 함수, `0x02`: 불법 주소, `0x03`: 불법 데이터 값)
    
- CRC-16 Modbus 다항식($A001_{hex}$) 계산 원리 및 룩업 테이블(LUT) 구현법
    

### Part 3: 클라이언트(Master) 구현 및 데이터 핸들링

- PC/임베디드 환경에서의 시리얼 통신 기초 (Baud, Parity, Stop bit, 버퍼 관리)
    
- `struct` 모듈을 활용한 바이너리 패킷 빌드 및 파싱 (Endianness, Signed/Unsigned)
    
- 부동소수점(Float 32-bit) 및 32-bit 정수 데이터 파싱 기법 (Word Swap, Byte Swap)
    
- 타임아웃, 재시도(Retry), 에러 복구 스테이트 머신(State Machine) 설계
    

### Part 4: 멀티 드롭(Multi-drop) 네트워크 & 실무 엔지니어링

- RS-485 멀티드롭 버스 토폴로지 구축 (데이지 체인, 종단 저항 120Ω의 역할)
    
- 복수 장비 폴링(Polling) 스케줄러 설계 (충돌 방지 및 대기 시간 튜닝)
    
- 가상 시뮬레이터(Modbus Poll, Modbus Slave)를 활용한 통신 디버깅 및 패킷 스니핑
    
- 파이썬 고수준 라이브러리(`pymodbus`) 활용 및 직접 만든 저수준 드라이버와의 성능/구조 비교