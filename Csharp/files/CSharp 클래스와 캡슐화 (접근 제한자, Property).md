캡슐화(Encapsulation)는 객체지향 프로그래밍(OOP)의 핵심 원칙 중 하나로, **객체의 내부 데이터(필드)를 외부에서 직접 수정하지 못하도록 숨기고(숨김성), 정해진 통로(메서드/프로퍼티)를 통해서만 안전하게 접근하도록 제어하는 기술**입니다.

## 1. 접근 제한자 (Access Modifiers)

외부에서 클래스의 멤버(변수, 메서드, 프로퍼티 등)에 어디까지 접근할 수 있는지를 통제합니다.

### 🔑 주요 접근 제한자 종류

| **접근 제한자**      | **접근 허용 범위**         | **설명**                               |
| --------------- | -------------------- | ------------------------------------ |
| **`private`**   | **클래스 내부만**          | 기본값. 외부 및 상속 관계에서도 직접 접근 불가 (데이터 보호) |
| **`public`**    | **어디서나 접근 가능**       | 클래스 외부, 다른 프로젝트/어셈블리에서도 접근 허용        |
| **`protected`** | **자신 + 상속받은 자식 클래스** | 클래스 내부 및 자신을 상속받은 자식 클래스에서만 접근 허용    |
| **`internal`**  | **동일한 프로젝트(어셈블리) 내** | 같은 프로젝트 내부에서는 어디서나 접근 가능             |
```cs
public class BankAccount
{
	private int _balance;
	
	public BankAccount(int amount)
	{
		this._balance = amount;
	}
	
	public int GetBalance()
	{
		return _balance;
	}
	
	public void SetBalance(int amount)
	{
		if (amount < 0)
		{
			Console.WriteLine("Balance can't be below zero");
			return;
		}
		
		_balance = amount;
		
	}
}

BankAccount account = new BankAccount(10000);
Console.WriteLine(account.GetBalance());
account.SetBalance(-29999);
```
**Getter/Setter 함수를 만드는 번거로움을 줄이기 위해 외부에서는 속성처럼 쓰면서 내부적으로 메서드처럼 작용하는 Property기능 제공**
#### 1. 기본 프로퍼티
```cs
public class Person
{
	private int _age;
	
	// 속성처럼 사용할 프로퍼티 설정
	public int Age
	{
		get // 읽기 요청이 오면 get 실행
		{
			return _age;
		}
		
		set // 쓰기 요청이 오면 set 실행
		{
			if (value < 0) // 외부에서 들어온 값을 예약어 value로 받음
				_age = 0;
			else
				_age = value;		
		}
	}
}

Person p = new Person();
p.Age = 25;
Console.WriteLine(p.Age);
p.Age = -30;
Console.WriteLine(p.Age);
```

#### 2. 자동 구현 프로퍼티
지금은 조건문이 없더라도 **C# 규격(인터페이스, 데이터 바인딩 등)에 맞추고 미래의 유연한 수정을 대비**하기 위해 쓴다.
```cs
public class Item
{
	public string Name {get; set;} // 접근제한자 public으로 읽기 쓰기 가능
	public int Price {get; set;}
	/* 추후에 setter 조건을 설정할 때 
	// public int Price {get; set;}
	private int _price;
	public int Price
	{
		get => _price
		set => _price = (value < 0) ? 0 : value;
	}
	*/
	
	public int Id {get; private set;} //읽기는 public 쓰기는 private
}

Item item = new Item() { Name = "potion", Price = 20000};
Console.WriteLine(item.Price);
```

---
*참고*
## 3. 읽기 전용 및 최신 프로퍼티 문법

> **💡 한 줄 요약:** _"계산해서 값만 내주거나, 처음 한 번 설정 후 절대 안 바뀌게 하고 싶을 때"_

### ① 계산 전용 읽기 프로퍼티 (`=>`)

별도의 변수에 값을 저장하지 않고, **요청할 때마다 공식을 계산해서 결과를 돌려주는 프로퍼티**입니다. `get { return ...; }`을 `=>` (람다 연산자)로 축약한 형태입니다.

C#

```
public class Circle
{
    public double Radius { get; set; } // 반지름

    // 별도의 변수가 없고, Radius가 바뀔 때마다 면적을 즉석 계산해서 가져옴 (get만 존재)
    public double Area => Math.PI * Radius * Radius; 
}

// 💻 사용법
Circle c = new Circle();
c.Radius = 5;
Console.WriteLine(c.Area); // 🖥️ 78.53... (즉석 계산되어 나옴)
// c.Area = 100; // 💥 에러! set이 없으므로 값을 대입할 수 없음
```

### ② 초기화 전용 프로퍼티 (`init`) - C# 9.0+

`set` 대신 `init`을 사용하면, **객체를 처음 생성할 때 딱 한 번만 값을 넣을 수 있고 그 이후에는 절대로 수정할 수 없게** 만듭니다. (데이터 변경으로 인한 버그를 방지할 때 최고입니다.)

C#

```
public class Character
{
    public string Name { get; init; } // 생성 시점에만 값 할당 가능!
}

// 💻 사용법
// 객체 생성 시 딱 한 번만 지정 가능!
Character hero = new Character { Name = "전사" }; 

// hero.Name = "마법사"; // 💥 에러! 생성 이후에는 수정 불가능!
```