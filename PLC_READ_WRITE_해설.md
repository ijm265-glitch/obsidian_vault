
### 1. 상단 선언부: 네임스페이스 로드 (Using)

C#

```
using System;
using System.IO.Ports;
using System.Threading;
using Modbus.Device;
```

- **`using`**: C# 키워드. 파이썬의 `import`처럼 특정 패키지(네임스페이스)를 현재 파일로 가져옵니다. 네임스페이스를 선언해 두면 매번 `System.Console` 대신 `Console`처럼 축약해 부를 수 있습니다.
    
      
    
- **`System`**: .NET BCL(Base Class Library) 최상위 네임스페이스. 콘솔, 예외, 기본 타입(`string`, `byte`, `ushort`) 관리.
    
      
    
- **`System.IO.Ports`**: 시리얼/RS-485 하드웨어 통신 제어 전용 네임스페이스.
    
      
    
- **`System.Threading`**: CPU 스레드 제어 및 지연(`Thread.Sleep`) 전용 네임스페이스.
    
      
    
- **`Modbus.Device`**: `NModbus4` 라이브러리에서 제공하는 모드버스 마스터/슬레이브 제어 네임스페이스.
    
      
    

### 2. 구조 선언부: 네임스페이스, 클래스, 진입점

C#

```
namespace PLC_Connect
{
    internal class Program
    {
        private static void Main(string[] args)
        {
```

- **`namespace PLC_Connect`**: 이 프로젝트 내 코드들의 논리적 그룹(폴더 역할). 다른 프로젝트와 클래스 이름 충돌을 방지합니다.
    
      
    
- **`internal class Program`**:
    
      
    - `internal`: 접근 제한자. 같은 프로젝트(어셈블리) 내부에서만 이 클래스를 볼 수 있습니다.
        
          
        
    - `class`: 객체지향의 기본 단위.
        
          
        
- **`private static void Main(string[] args)`**:
    
      
    - `private`: 이 메서드는 `Program` 클래스 내부에서만 호출 가능.
        
          
        
    - `static`: 정적 메서드. 윈도우 OS가 `new Program()` 인스턴스를 만들지 않고도 메모리 고정 주소를 찾아 바로 기동할 수 있습니다.
        
          
        
    - `void`: 반환값 없음 (`return` 생략 가능).
        
          
        
    - `Main`: 런타임이 무조건 첫 번째로 찾는 진입점(Entry Point).
        
          
        
    - `string[] args`: 터미널 실행 시 전달되는 커맨드 라인 인자 배열.
        
          
        

### 3. 변수 정의 및 하드웨어 포트 인스턴스화

C#

```
            string portName = "COM6";
            int baudRate = 115200;
            byte slaveId = 1; // XG5000 채널 2 국번
```

- **`string`, `int`, `byte`**: 정적 데이터 타입.
    
      
    - `byte`: 8비트 부호 없는 정수 (0~255). Modbus 국번(Slave ID)은 1바이트 규격이므로 `byte` 타입을 사용합니다.
        
          
        

C#

```
            using (SerialPort port = new SerialPort(portName, baudRate, Parity.None, 8, StopBits.One))
            {
                port.ReadTimeout = 1000;
                port.WriteTimeout = 1000;
```

- **`using ( ... )`**: `IDisposable` 인터페이스를 구현한 객체 전용 리소스 관리 문법.
    
      
    - 시리얼 포트는 OS가 특정 프로세스에 독점권을 주는 하드웨어 자원입니다.
        
          
        
    - `using` 블록을 빠져나가는 순간(정상 종료든 에러 발생이든) 즉시 `port.Dispose()`를 내부적으로 실행하여 OS 포트 점유를 해제합니다. (파이썬의 `with` 구문과 동일)
        
          
        
- **`new SerialPort(...)`**:
    
      
    - 패키지: `System.IO.Ports`
        
          
        
    - 클래스: `SerialPort`
        
          
        
    - 메서드: **생성자(Constructor)**. 통신 포트 객체를 힙(Heap) 메모리에 할당하고 초기 파라미터를 주입합니다.
        
          
        
    - 인자:
        
          
        - `Parity.None`: 패리티 비트 사용 안 함 (`Parity` 열거형).
            
              
            
        - `8`: 데이터 비트 수.
            
              
            
        - `StopBits.One`: 정지 비트 1 (`StopBits` 열거형).
            
              
            
- **`port.ReadTimeout = 1000;` / `port.WriteTimeout = 1000;`**:
    
      
    - 클래스 속성(Property) 설정.
        
          
        
    - 읽기/쓰기 시 PLC 응답이 1000ms(1초) 이상 지연되면 무한정 대기(Freeze)하지 않고 `TimeoutException`을 발생시킵니다.
        
          
        

### 4. 물리 포트 개방 및 모드버스 엔진 결합

C#

```
                try
                {
                    port.Open();
                    Console.WriteLine($"[1] {portName} 시리얼 포트 열기 성공!");

                    using (IModbusSerialMaster master = ModbusSerialMaster.CreateRtu(port))
                    {
```

- **`port.Open()`**:
    
      
    - `SerialPort` 클래스의 인스턴스 메서드.
        
          
        
    - 윈도우 OS 커널의 `CreateFile` API를 호출해 실제 COM6 포트를 통신 가능 상태로 엽니다.
        
          
        
- **`Console.WriteLine($"...")`**:
    
      
    - 패키지: `System`
        
          
        
    - 클래스: `Console`
        
          
        
    - 메서드: `WriteLine` (정적 메서드)
        
          
        
    - `$"..."`: C# 6.0부터 도입된 문자열 보간(String Interpolation). 파이썬의 f-string(`f"..."`)과 완벽히 동일합니다.
        
          
        
- **`ModbusSerialMaster.CreateRtu(port)`**:
    
      
    - 패키지: `Modbus.Device`
        
          
        
    - 클래스: `ModbusSerialMaster`
        
          
        
    - 메서드: **팩토리 정적 메서드(Factory Static Method)**.
        
          
        
    - 열려 있는 물리 시리얼 스트림(`port`)을 입력받아, 그 위에 Modbus RTU 프로토콜 패킷 포맷터(슬레이브 국번, Function Code, 레지스터 번지, CRC-16 계산 엔진)를 조립한 `IModbusSerialMaster` 인터페이스 객체를 반환합니다.
        
          
        
- **`IModbusSerialMaster master`**:
    
      
    - 인터페이스 타입. 이름 앞의 `I`는 C#에서 인터페이스(규격서)를 의미하는 네이밍 규칙입니다.
        
          
        

### 5. D 레지스터(워드) 제어: Function 0x06 & 0x03

C#

```
                        // 1. D00000 레지스터에 값 쓰기 및 읽기
                        ushort d0Address = 0; // D0
                        ushort writeValue = 1234;

                        Console.WriteLine($"\n[2] D0 영역에 값 쓰기 ({writeValue})...");
                        master.WriteSingleRegister(slaveId, d0Address, writeValue);
                        Console.WriteLine("-> D0 쓰기 완료");

                        ushort[] registers = master.ReadHoldingRegisters(slaveId, d0Address, 1);
                        Console.WriteLine($"-> D0 읽어온 값: {registers[0]}");
```

- **`ushort`**: 16비트 부호 없는 정수 (Unsigned Short, 0~65,535). PLC의 1개 워드(Word = 16-bit) 크기와 정확히 1:1 대응합니다.
    
      
    
- **`d0Address = 0;`**:
    
      
    - XG5000 모드버스 설정에서 워드 시작 주소를 `D00000`으로 잡았기 때문에, 오프셋 `0`은 래더의 `D00000`을 가리킵니다.
        
          
        
- **`master.WriteSingleRegister(slaveId, d0Address, writeValue);`**:
    
      
    - 클래스: `IModbusSerialMaster`
        
          
        
    - 메서드: `WriteSingleRegister`
        
          
        
    - **Modbus Function Code 0x06 (Preset Single Register)** 프레임을 생성해 시리얼로 송신합니다.
        
          
        
    - PLC의 1번 국번, 0번지 레지스터에 16비트 값 `1234`를 씁니다.
        
          
        
- **`master.ReadHoldingRegisters(slaveId, d0Address, 1);`**:
    
      
    - 메서드: `ReadHoldingRegisters`
        
          
        
    - **Modbus Function Code 0x03 (Read Holding Registers)** 프레임을 송신합니다.
        
          
        
    - 슬레이브 `1`번의 `d0Address(0)`부터 `1`개의 워드를 읽어와 `ushort[]` 배열 형태로 반환합니다.
        
          
        
- **`registers[0]`**: 배열의 첫 번째 요소이므로 `D0`에 실제로 기록되어 있는 값을 추출합니다.
    
      
    

### 6. M 접점(비트/코일) 제어: Function 0x05 & 0x01

C#

```
                        // 2. M00000 접점 ON/OFF 제어
                        ushort m0Address = 0; // M0

                        Console.WriteLine($"\n[3] M0 코일 ON...");
                        master.WriteSingleCoil(slaveId, m0Address, true);
                        bool[] coils = master.ReadCoils(slaveId, m0Address, 1);
                        Console.WriteLine($"-> M0 상태: {coils[0]}");

                        Thread.Sleep(1000);

                        Console.WriteLine($"\n[4] M0 코일 OFF...");
                        master.WriteSingleCoil(slaveId, m0Address, false);
                        coils = master.ReadCoils(slaveId, m0Address, 1);
                        Console.WriteLine($"-> M0 상태: {coils[0]}");
```

- **`master.WriteSingleCoil(slaveId, m0Address, true);`**:
    
      
    - 메서드: `WriteSingleCoil`
        
          
        
    - **Modbus Function Code 0x05 (Force Single Coil)** 프레임을 송신합니다.
        
          
        
    - XG5000 모드버스 비트 시작 주소가 `M00000`이므로, `m0Address = 0`에 `true`(ON: 0xFF00) 신호를 보내 접점을 켭니다.
        
          
        
- **`master.ReadCoils(slaveId, m0Address, 1);`**:
    
      
    - 메서드: `ReadCoils`
        
          
        
    - **Modbus Function Code 0x01 (Read Coils)** 프레임을 송신합니다.
        
          
        
    - `0`번지 비트부터 1개의 코일 상태를 읽어와 `bool[]` (참/거짓 배열) 형태로 반환합니다.
        
          
        
- **`Thread.Sleep(1000);`**:
    
      
    - 패키지: `System.Threading`
        
          
        
    - 클래스: `Thread`
        
          
        
    - 메서드: `Sleep` (정적 메서드)
        
          
        
    - 현재 실행 흐름을 1000ms(1초)간 일시정지시킵니다. (PLC 접점이 ON 상태를 1초간 유지하도록 지연 생성)
        
          
        
- **`master.WriteSingleCoil(slaveId, m0Address, false);`**:
    
      
    - 0x05 코드를 다시 날려 M0 접점을 `false`(OFF: 0x0000)로 리셋합니다.
        
          
        

### 7. 예외 처리(Exception) 및 리소스 청소 (Finally)

C#

```
                }
                catch (TimeoutException)
                {
                    Console.WriteLine("\n[에러] 응답 타임아웃 발생: PLC 응답이 없습니다.");
                    Console.WriteLine("조치: USB 컨버터의 A선과 B선 배선을 맞바꿔보세요.");
                }
                catch (Exception ex)
                {
                    Console.WriteLine($"\n[오류 발생]: {ex.Message}");
                }
                finally
                {
                    if (port.IsOpen)
                    {
                        port.Close();
                    }
                }
            }
```

- **`catch (TimeoutException)`**:
    
      
    - 패키지: `System`
        
          
        
    - `ReadTimeout(1000ms)` 안에 PLC로부터 정상적인 CRC 패킷 응답이 오지 않았을 때 발생하는 전용 예외입니다.
        
          
        
    - 배선 단선, A/B 극성 반대, 국번 불일치 시 이 블록으로 분기합니다.
        
          
        
- **`catch (Exception ex)`**:
    
      
    - 모든 예외 클래스의 부모 클래스인 `Exception`을 잡아내는 범용 블록. 포트 충돌, 권한 부족 등 알 수 없는 런타임 오류 메시지를 `ex.Message`로 안전하게 출력합니다.
        
          
        
- **`finally`**:
    
      
    - `try`가 성공하든, `catch`로 에러가 나든 **무조건 마지막에 실행되는 보장 블록**입니다.
        
          
        
- **`port.IsOpen` / `port.Close()`**:
    
      
    - `port.IsOpen` 속성으로 포트가 열려 있는지 확인하고, 열려 있다면 `Close()`로 닫아줍니다.
        
          
        
    - 비록 바깥에 `using (SerialPort ...)`이 감싸고 있어 최종적으로 Dispose되지만, 통신 에러 발생 즉시 물리 포트를 닫아주는 2중 안전장치 역할을 합니다.
        
          
        

### 8. 프로그램 대기 종료

C#

```
            Console.WriteLine("\n종료하려면 아무 키나 누르세요...");
            Console.ReadKey();
        }
    }
}
```

- **`Console.ReadKey()`**:
    
      
    - 키보드 입력을 1글자 받을 때까지 실행을 대기(Block)시킵니다.
        
          
        
    - 콘솔 창이 실행을 끝내고 번쩍하고 바로 닫혀버리는 현상을 방지합니다.