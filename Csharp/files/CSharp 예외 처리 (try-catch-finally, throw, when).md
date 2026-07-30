C#에서 예외 처리(Exception Handling)는 프로그램 실행 중에 발생할 수 있는 오류를 안전하게 처리하여 앱이 튕기지(다운되지) 않도록 보호하는 핵심적인 문법입니다.

요청하신 **`try-catch-finally`**, **`throw`**, 그리고 C#의 매력적인 기능인 **`when`** 키워드까지 깔끔하게 정리해 드리겠습니다!
## 1. 기본 구조: `try-catch-finally'
- **`try`**: 예외가 발생할 가능성이 있는 코드를 작성하는 블록
- **`catch`**: 예외가 발생했을 때 이를 잡아서 처리하는 블록
- **`finally`**: 예외 발생 여부와 상관없이 **무조건 실행되는 정리(Clean-up) 블록** (파일 닫기, DB 연결 해제 등)
```cs
try
{
    int a = 10;
    int b = 0;
    int result = a / b; // ❌ 0으로 나누려고 하므로 DivideByZeroException 발생!
}
catch (DivideByZeroException ex)
{
    // 특정 예외만 받아서 처리
    Console.WriteLine($"0으로 나눌 수 없습니다: {ex.Message}");
}
catch (Exception ex)
{
    // 모든 예외의 최상위 부모 클래스 (위에서 안 걸린 나머지 모든 예외 처리)
    Console.WriteLine($"알 수 없는 에러 발생: {ex.Message}");
}
finally
{
    // 성공하든, catch로 빠지든 무조건 실행됨
    Console.WriteLine("작업을 종료하고 자원을 정리합니다.");
}
```

> **💡 주의:** `catch` 블록은 여러 개 작성할 수 있으며, **하위 예외(구체적인 예외)부터 상위 예외(범용적인 `Exception`) 순서**로 배치해야 합니다.

## 2. 예외 발생 및 전달: `throw`

`throw`는 두 가지 목적으로 쓰입니다.
1. **의도적으로 예외를 직접 발생시킬 때**
2. **catch한 예외를 위쪽(나를 호출한 메서드)으로 다시 던질 때 (Re-throw)**
### ① 의도적으로 예외 던지기

잘못된 데이터가 들어왔을 때 프로그램 진행을 즉시 중단하고 에러를 알립니다.
```cs
public void SetAge(int age)
{
    if (age < 0)
    {
        // ArgumentException 예외 객체를 직접 생성하여 던짐
        throw new ArgumentException("나이는 음수가 될 수 없습니다.");
    }
    Console.WriteLine($"나이가 {age}세로 설정되었습니다.");
}
```

### ② 예외 다시 던지기 (Re-throw) - ★ 실무 주의사항

`catch` 블록 안에서 `throw;`와 `throw ex;`는 엄청난 차이가 있습니다.
```cs
try
{
    // ... 실행 코드
}
catch (Exception ex)
{
    Log(ex.Message); // 로그만 남기고

    // ⭕ 추천: 원본 에러의 스택 트레이스(어느 라인에서 터졌는지)를 그대로 보존하며 전달
    throw; 

    // ❌ 비추천: 스택 트레이스가 끊겨서 '여기(catch 위치)'부터 다시 시작된 것으로 바뀌어 버림 (추적 어려움)
    // throw ex; 
}
```

## 3. 필터링의 꽃: `when` 필터 (C# 6.0+)

`when` 키워드를 사용하면 `catch` 블록에 **조건식**을 붙일 수 있습니다.
예외의 종류뿐만 아니라 "예외의 상세 상태나 특정 조건"까지 만족할 때만 해당 `catch` 블록을 실행하도록 만드는 아주 유용한 기능입니다.
### 예시 1: HTTP 상태 코드나 에러 코드 조건 처리
```cs
try
{
    FetchDataFromDataServer();
}
// HttpRequestException 중에서도 StatusCode가 404(Not Found)일 때만 잡음!
catch (HttpRequestException ex) when (ex.StatusCode == System.Net.HttpStatusCode.NotFound)
{
    Console.WriteLine("요청하신 페이지나 데이터를 찾을 수 없습니다 (404).");
}
// StatusCode가 500 계열일 때만 잡음!
catch (HttpRequestException ex) when ((int)ex.StatusCode >= 500)
{
    Console.WriteLine("서버 내부 오류입니다. 잠시 후 다시 시도해 주세요.");
}
```

### 예시 2: 특정 환경/조건에 따른 예외 처리
```cs
bool isDebugMode = true;

try
{
    ProcessImportantTask();
}
// 디버그 모드가 아닐 때만 catch를 타고, 디버그 모드면 catch를 건너뛰어 IDE가 에러 지점을 바로 잡게 함
catch (Exception ex) when (!isDebugMode)
{
    SendErrorToAdmin(ex);
}
```

## 4. C# 예외 처리 한눈에 정리

1. **`try-catch-finally`**: 안전하게 코드를 실행하고 자원을 반납하는 기본 틀.
2. **`throw`**: 에러 상황을 명시적으로 던지거나, 다시 위로 전달(`throw;`).
3. **`when`**: `catch`에 조건문을 붙여 원하는 세부 예외 상황만 골라서 스마트하게 처리.

> **💡 실무 팁 (`using` 문과의 관계):**
> 
> `finally` 블록에서 `Close()`나 `Dispose()`를 부르는 작업은 C#의 `using` 키워드(`using (var stream = ... )`)를 쓰면 컴파일러가 자동으로 `try-finally`로 변환해 줍니다!