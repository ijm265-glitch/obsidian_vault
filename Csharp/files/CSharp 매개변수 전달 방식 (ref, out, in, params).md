C#에서 메서드로 인자(Argument)를 전달할 때 기본 동작은 값에 의한 전달(Pass by Value)입니다. 즉, 변수의 복사본이 전달됩니다.

하지만 메모리 접근 제어나 성능 최적화, 혹은 여러 개의 값을 반환받기 위해 **참조에 의한 전달(Pass by Reference)** 키워드인 `ref`, `out`, `in`과 가변 인자를 위한 `params` 키워드를 제공합니다.

## 1. 키워드 한눈에 보기

|**키워드**|**방향**|**메서드 내부에서의 수정**|**호출 전 초기화 필수 여부**|**주요 사용 목적**|
|---|---|---|---|---|
|**기본 (None)**|입력 (In)|수정해도 원본에 영향 없음|필수|일반적인 값 전달 (복사)|
|**`ref`**|입출력 (In/Out)|**수정 가능** (원본 반영)|**필수**|원본 변수의 값을 직접 읽고 수정할 때|
|**`out`**|출력 (Out)|**반드시 값 할당 필요**|불필요 (메서드가 채워줌)|메서드에서 여러 개의 값을 반환받을 때|
|**`in`**|읽기 전용 (In)|**수정 불가능** (컴파일 에러)|**필수**|대용량 값 타입 복사 비용 방지 (성능 최적화)|
|**`params`**|입력 (In)|일반 배열처럼 사용|선택|가변 개수의 인자를 전달받을 때|
### ① `ref` (Reference: 참조 전달)
- **메서드 외부의 변수 주소를 직접 넘겨줍니다.**
- 메서드 내부에서 값을 변경하면 **호출한 쪽의 원본 변수 값도 함께 변경**됩니다.
- 호출하기 전에 **반드시 변수가 초기화**되어 있어야 합니다.
    
```cs
void Swap(ref int a, ref int b)
{
    int temp = a;
    a = b;
    b = temp;
}

int x = 10;
int y = 20;

// 호출 시에도 ref 키워드를 명시해야 함
Swap(ref x, ref y);

Console.WriteLine($"x: {x}, y: {y}"); // 출력: x: 20, y: 10 (원본이 바뀌어 있음)
```

### ② `out` (Output: 출력 전용 참조)

- 메서드로부터 **결과 값을 바깥으로 꺼내오기 위해** 사용합니다.
- 호출하는 쪽에서 변수를 미리 초기화할 필요가 없습니다.
- **메서드 내부에서 반드시 `out` 매개변수에 값을 할당(초기화)해야 합니다.** (안 하면 컴파일 에러)
- 함수에서 2개 이상의 결과를 한 번에 내보내야 할 때 (원칙적으로 반환값이 1개임)
- 성공여부와 실제 데이터를 깔끔하게 처리하기 위해
```cs
// TryParse와 같이 성공 여부(bool)와 결과값(out)을 함께 받을 때 자주 사용
bool Divide(int a, int b, out int result)
{
    if (b == 0)
    {
        result = 0; // out 변수는 메서드가 끝나기 전에 반드시 값을 채워야 함!
        return false;
    }

    result = a / b;
    return true;
}

// C# 7.0부터는 인자 전달 위치에서 변수 선언 가능 (out var)
if (Divide(10, 2, out int res))
{
    Console.WriteLine($"결과: {res}"); // 출력: 결과: 5
}

// return만으로는 몫과 나머지 2개를 동시에 넘겨줄 수 없음!
// out을 쓰면 여러 개의 결과값을 바깥으로 내보낼 수 있음
void Divide(int dividend, int divisor, out int quotient, out int remainder)
{
    quotient = dividend / divisor; // 몫
    remainder = dividend % divisor; // 나머지
}

// 호출하는 쪽
Divide(10, 3, out int q, out int r);
Console.WriteLine($"몫: {q}, 나머지: {r}"); // 몫: 3, 나머지: 1

string input = "123";

// TryParse는 성공했는지(true/false)를 return하고,
// 변환된 진짜 숫자 결과는 out으로 받아옴
if (int.TryParse(input, out int number))
{
    Console.WriteLine($"변환 성공: {number}");
}
else
{
    Console.WriteLine("숫자가 아닌 문자열입니다.");
}
```

### ③ `in` (ReadOnly Reference: 읽기 전용 참조)

- `ref`처럼 참조(주소)로 넘기지만, 메서드 내부에서 값을 수정하지 못하게 강제(`readonly`)합니다.
- 크기가 큰 `struct`(구조체) 등을 매개변수로 전달할 때, **값 복사로 인한 메모리/CPU 성능 저하를 막기 위해** 사용합니다.

```cs
struct BigData
{
    public double X, Y, Z; // 크기가 큰 구조체
}

void ProcessData(in BigData data)
{
    // data.X = 10; // 💥 컴파일 에러! (in 변수는 수정 불가)
    Console.WriteLine(data.X); // 읽기만 가능 (복사본 생성 없이 주소로 읽어옴)
}
```d

### ④ `params` (Parameter Array: 가변 인자)

- 메서드에 **개수가 정해지지 않은 여러 개의 인자**를 배열 형태로 넘길 수 있게 해줍니다.
- 메서드 매개변수 목록의 **가장 마지막**에 위치해야 합니다.
```cs
int SumAll(params int[] numbers)
{
    int total = 0;
    foreach (int n in numbers)
    {
        total += n;
    }
    return total;
}

// 콤마(,)로 개수 제한 없이 전달 가능 (컴파일러가 자동으로 배열로 묶어줌)
Console.WriteLine(SumAll(1, 2, 3));        // 출력: 6
Console.WriteLine(SumAll(10, 20, 30, 40)); // 출력: 100
```