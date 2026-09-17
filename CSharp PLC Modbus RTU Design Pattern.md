
`using System.IO.Ports`
1. 시리얼 포트 객체 생성 (`SerialPort`)
2. 포트 개방 (`port.Open()`)
---
`using Modbus.Device` 
3. master 객체 생성 (`ModbusSerialMaster.CreateRtu(port)`) 
4. master 객체의 메서드로 쓰기, 읽기 작업 수행 (`ReadHoldingRegisters`, `WriteSingleRegister` 등) 
5. 통신 종료 및 자원 해제 (`Dispose`, `Close` 또는 `using` 블록)