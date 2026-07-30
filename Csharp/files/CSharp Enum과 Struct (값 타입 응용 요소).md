## 1. Enum (열거형: Enumeration)

`enum`은 **관련된 정수 상수들의 집합에 가독성 높은 이름을 붙여 관리**하는 값 타입입니다.
### 🔑 주요 특징
- **기본 정수 타입:** 명시하지 않으면 내부적으로 `int` (4byte) 타입을 사용합니다. (필요시 `byte`, `short` 등으로 변경 가능)
- **가독성 및 안전성:** 의미를 알 수 없는 '마법의 숫자(Magic Number)' 대신 명확한 이름으로 코드를 작성할 수 있어 가독성이 높아지고 오타로 인한 버그를 방지합니다.
```cs
enum PlayerState
{
	Idle = 0,
	Walking = 1,
	Running = 2,
	Dead = 99
}

PlayerState currentstate = PlayerState.Idle;

int statevalue = (int)currentstate;

Console.WriteLine($"currentstate: {currentstate}"); // currentstate.ToString()을 출력한 형태
Console.WriteLine($"statevalue: {statevalue}");
```

```cs
[System.Flags]
enum EquipmentSlot
{
	None = 0,
	Weapon = 1 << 0,
	Armor = 1 << 1,
	Helmet = 1 << 2,
	Ring = 1 << 3
}

EquipmentSlot myEquip = EquipmentSlot.Weapon|EquipmentSlot.Helmet;

if (myEquip.HasFlag(EquipmentSlot.Weapon))
{
	Console.WriteLine("There is a Weapon");
}
```
## 2. Struct (구조체)

`struct`는 관련 데이터를 하나로 묶는 **작고 가벼운 값 타입 객체**입니다. 클래스(`class`)와 문법적으로 유사하지만 메모리 할당 및 복사 방식에서 큰 차이가 있습니다.
### 🔑 주요 특징
- **스택(Stack) 할당:** `new` 키워드로 생성하더라도 클래스처럼 힙에 할당되지 않고 **스택 메모리에 할당**됩니다. (단, 클래스의 내부 필드로 선언된 구조체는 클래스와 함께 힙에 위치함)
- **값 복사 (Value Copy):** 다른 변수에 대입하거나 메서드 인자로 전달할 때 **데이터 전체가 깊은 복사(Deep Copy) 형태로 복제**됩니다. 
- **상속 불가:** 다른 구조체나 클래스를 상속할 수 없습니다. (단, **인터페이스 구현은 가능**)
```cs
struct Vector2D
{
	public float X;
	public float Y;
	
	public Vector2D(float x, float y)
	{
		this.X = x;
		this.Y = y;
	}
	
	public void status()
	{
		Console.WriteLine($"x: {X}, y: {Y}");
	}
}

Vector2D v1 = new Vector2D(10.0f, 203.4f);
Vector2D v2 = v1; // value type이기 때문에 deepcopy발생

v2.X = 99.0f;

Console.WriteLine(v1.X); // 203.4
Console.WriteLine(v2.X); // 99 - 독립된 값을 가짐

```