C#의 패턴 매칭(Pattern Matching)은 객체의 **타입, 형태, 내부 값의 조건**을 검사하고, 검사된 값을 즉시 변수에 할당하거나 분기 처리하는 강력한 문법입니다.

## 1. is 연산자
### 1. 선언 패턴 (Type Pattern)
- 타입을 확인하고, 매칭에 성공하면 자동으로 형변환된 새로운 변수 생성
```cs
object obj = "Hello C#";
// 기존방식: is로 검사하고, (string)으로 형변환
if (obj is string)
{
	string s1 = (string)obj;
	Console.WriteLine(s1.Length);
}

if (obj is string s2)
{
	Console.WriteLine(s2.Length);
}
```

### 2. 관계 및 논리 패턴 (Relational & Logical Patterns)
숫자의 범위나 조건들을 `and`, `or`, `not` 키워드로 조합할 수 있음
```cs
int age = 25;

if (age is >= 18 and <= 65)
{
	Console.WriteLine("True");
}

object input = "null";

if (input is not null)
{
	Console.WriteLine("Null: False.");
}
```

### 3. 속성 패턴 (Property Pattern)
객체 내부의 필드나 프로퍼티 값을 직접 검사하여 매칭
```cs
// 1. 아직 get/set을 안 배웠다면? -> 그냥 일반 변수처럼 생각하세요!
class Person
{
    public string Name; // 일반 변수(필드)
    public int Age;     // 일반 변수(필드)
}

// 2. 사용할 때도 그냥 똑같이 변수에 값 넣고 쓰시면 됩니다.
Person p = new Person();
p.Name = "Alice";
p.Age = 20;

// 3. 패턴 매칭도 똑같이 동작합니다!
if (p is Person { Age: >= 18 } adult)
{
    Console.WriteLine($"{adult.Name}님은 성인입니다.");
}
```
## 2. `switch` 식 (Switch Expression) - C# 8.0+

기존의 길고 번거로웠던 `switch` 문을 값을 반환하는 깔끔한 식(Expression)으로 단축시킨 문법입니다. 화살표(`=>`) 기호를 사용합니다.

### 🔑 기본 문법 비교

```cs
int day = 3;

// 기존 switch 문 (Statement)
string dayName;
switch (day)
{
    case 1: dayName = "월요일"; break;
    case 2: dayName = "화요일"; break;
    case 3: dayName = "수요일"; break;
    default: dayName = "주말"; break;
}

// 최신 switch 식 (Expression)
string result = day switch
{
    1 => "월요일",
    2 => "화요일",
    3 => "수요일",
    _ => "주말" // _ (언더스코어)는 default 역할을 함
};
```

## 3. `switch` 식에서의 고급 패턴 응용

### ① 위치 패턴 및 튜플 매칭 (Tuple Pattern)

여러 개의 값을 튜플(`()`)로 묶어 한 번에 상태를 분기할 수 있습니다.

```cs
string GetState(bool isOnline, bool hasPermission) => (isOnline, hasPermission) switch
{
    (true, true)   => "접근 허용",
    (true, false)  => "권한 부족",
    (false, _)     => "오프라인 상태",
};
```

### ② 가드 절 (`when` 조건문)

패턴에 들어맞더라도 **추가적인 조건 검사**가 필요할 때 `when` 키워드를 붙입니다.
```cs
int CalculateDiscount(int price, bool isVIP) => price switch
{
    >= 100000 when isVIP => 20, // 10만원 이상이면서 VIP이면 20% 할인
    >= 100000            => 10, // 10만원 이상이면 10% 할인
    _                    => 0
};
```

### ③ 위치 지정 패턴 (Positional Pattern) - 구조체/클래스 해체

`Deconstruct` 메서드가 정의된 구조체나 클래스는 튜플처럼 위치로 해체하여 매칭할 수 있습니다.
```cs
readonly struct Point
{
    public int X { get; };
    public int Y { get; };

    public Point(int x, int y) => (X, Y) = (x, y);

    // 해체자(Deconstructor)
    public void Deconstruct(out int x, out int y) => (x, y) = (X, Y);
}

Point pt = new Point(0, 5);

string quadrant = pt switch
{
    (0, 0) => "원점",
    (0, _) => "Y축 위",
    (_, 0) => "X축 위",
    _      => "사분면 위"
};
```