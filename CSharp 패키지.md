### 1. `using System;`
.NET의 가장 기본적이고 필수적인 핵심 기능들이 들어있는 최상위 네임스페이스입니다.
- **파이썬 대응**: 기본 내장 함수(`print`, `str`, `int`) 및 표준 라이브러리
- **주요 사용 클래스**:
    - `Console`: 터미널 화면 입출력 (`Console.WriteLine`)
    - `DateTime`: 타임스탬프 로깅 (`DateTime.Now`)
    - `Exception`: 에러 감지 및 예외 처리 클래스
    - 기본 자료형 메타데이터 (`Int32`, `Byte`, `String` 등)

### 2. `using System.IO.Ports;`
PC의 물리적인 시리얼 통신 포트(COM Port)를 열고 닫거나 신호를 주고받기 위한 통신 하드웨어 제어용 네임스페이스입니다.
- **파이썬 대응**: `import serial` (pyserial)
- **주요 사용 클래스**:
    - `SerialPort`: COM 포트 설정(COM6, 115200 bps, Parity, StopBit 등) 및 하드웨어 버퍼 오픈(`Open()`), 종료(`Close()`) 제어

### 3. `using System.Threading;
스레드(Thread)를 직접 다루고 시간 지연, 동기화 처리를 하기 위한 네임스페이스입니다.
- **파이썬 대응**: `import time`, `import threading`
- **주요 사용 기능**:
    - `Thread.Sleep(100)`: 파이썬의 `time.sleep(0.1)`과 동일하게 현재 스레드를 지정한 밀리초(ms) 동안 일시 정지시켜 CPU 점유율 폭주를 방지

### 4. `using Modbus.Device;`
NuGet으로 별도 설치한 **NModbus4** 패키지에서 제공하는 모드버스 프로토콜 전용 엔진 네임스페이스입니다.
- **파이썬 대응**: `from pymodbus.client import ModbusSerialClient`
- **주요 사용 인터페이스 및 클래스**:
    - `IModbusSerialMaster`: Modbus Master(Client) 동작 규격을 정의한 인터페이스
    - `ModbusSerialMaster.CreateRtu(...)`: 시리얼 포트 객체를 감싸서 Modbus RTU 바이너리 프레임(슬레이브 국번, Function Code, 레지스터 번지, CRC-16 계산 등)을 자동으로 생성하고 통신 패킷을 주고받는 클래스