- 추상 클래스(abstract class)와 인터페이스(interface)는 모두 클래스가 구현해야 할 메서드나 속성을 정의하는 도구이지만 개념적인 목적과 활용법에서 큰 차이가 있음

|**구분**|**추상 클래스 (abstract class)**|**인터페이스 (interface)**|
|---|---|---|
|**개념적 관계**|**Is-A** (예: 강아지는 동물이다)|**Can-Do** (예: 로봇과 사람은 날 수 있다)|
|**다중 상속**|**불가능** (단일 상속만 지원)|**가능** (여러 인터페이스 구현 가능)|
|**필드 (변수)**|멤버 변수(필드) 선언 **가능**|필드 선언 **불가능** (프로퍼티는 가능)|
|**접근 제한자**|`public`, `protected`, `private` 등 다양하게 사용 가능|기본적으로 `public` (C# 8.0 이상에서 구현체 가능)|
|**생성자**|생성자(`Constructor`) 가질 수 있음|생성자 가질 수 없음|
|**공통 코드**|코드 구현체(일반 메서드) 포함 가능|C# 8.0부터 기본 구현(Default Implementation) 가능|
### 1. 추상 클래스 (Abstract Class)
- 비슷한 부류의 클래스들 간에 코드(상태와 로직)을 공유하고 싶을 떄 사용
```cs
public abstract class Animal
{
	public string Name {get; set;} // 필드와 프로퍼티 보유 가능
	
	public Animal(string name) => Name = name;
	
	// 자식 클래스가 반드시 구현해야 할 메서드 
	public abstract void MakeSound();
}

public class Dog : Animal
{
	public Dog(string name) : base(name) { }
	
	public override void MakeSound() => Console.WriteLine("Bark!");
}

Dog dog = new Dog("Jindo");
dog.MakeSound();
```

### 2. 인터페이스 (Interface)
- 다른 클래스들이라고 특정 행동/기능을 동일한 규칙으로 수행하도록 강제할 때 사용
- 동시에 여러개가 구현 가능
```cs
public abstract class Animal
{
	public string Name {get; set;} // 필드와 프로퍼티 보유 가능
	
	public Animal(string name) => Name = name;
	
	// 자식 클래스가 반드시 구현해야 할 메서드 
	public abstract void MakeSound();
}

public interface IFlyable
{
	void Fly();
}

public interface IShootable
{
	void Shoot();
}

public class Bird : Animal, IFlyable
{
	public Bird(string name) : base(name) {}
	
	public override void MakeSound() => Console.WriteLine("ppp");
	public void Fly() => Console.WriteLine("bird is flying away..");
}

public class CombatDrone : IFlyable, IShootable
{
	public void Fly() => Console.WriteLine("Drone is flying");
	public void Shoot() => Console.WriteLine("I can see a projectile flying to me... fuck");
}

CombatDrone drone1 = new CombatDrone();
drone1.Fly();
drone1.Shoot();
```
- **인터페이스를 타입처럼 사용**
```cs

// Interface정의 
public interface IMouse
{
	void LeftClick();
	void RightClick();
	void Move(int x, int y);
}

// 1. 로지텍 마우스
public class LogitechMouse : IMouse
{
    public void LeftClick() => Console.WriteLine("로지텍 마우스: 딸깍 (좌클릭)");
    public void RightClick() => Console.WriteLine("로지텍 마우스: 딸깍 (우클릭)");
    public void Move(int x, int y) => Console.WriteLine($"로지텍 마우스: ({x}, {y}) 위치로 부드럽게 이동");
}

// 2. 레이저 게이밍 마우스
public class RazerMouse : IMouse
{
    public void LeftClick() => Console.WriteLine("레이저 마우스: 광학 스위치 딸깍!");
    public void RightClick() => Console.WriteLine("레이저 마우스: 광학 스위치 딸깍!");
    public void Move(int x, int y) => Console.WriteLine($"레이저 마우스: RGB 조명을 반짝이며 ({x}, {y}) 이동");
}

public class Laptop
{
	private IMouse mouse;
	
	public Laptop(IMouse mouse) => this.mouse = mouse;
	
	// Interface를 따르기 떄문에 mouse객체가 반드시 LeftClick, Move method가 구현되어 있음을 확신함
	public void UseMouse()
	{
		mouse.LeftClick();
		mouse.Move(10, 20);
	}
}

// 로지텍 마우스를 꽂은 노트북
Laptop laptop1 = new Laptop(new LogitechMouse());
laptop1.UseMouse(); 
// 출력:
// 로지텍 마우스: 딸깍 (좌클릭)
// 로지텍 마우스: (10, 20) 위치로 부드럽게 이동

// 레이저 마우스를 꽂은 노트북 (Laptop 코드는 1도 안 바뀜!)
Laptop laptop2 = new Laptop(new RazerMouse());
laptop2.UseMouse();
// 출력:
// 레이저 마우스: 광학 스위치 딸깍!
// 레이저 마우스: RGB 조명을 반짝이며 (10, 20) 이동




```

## [[Abstract class, Interface 선택 가이드]]
### **추상 클래스를 선택해야 할 때**

- 상속받을 클래스들이 **밀접하게 연관**되어 있을 때 (예: `Dog`, `Cat` $\rightarrow$ `Animal`)
- 클래스 간에 공통 상태(변수/필드)나 **공통 코드**를 많이 공유하고 싶을 때
- 나중에 공통 기능(일반 메서드)을 추상 클래스에 추가하여 하위 클래스에 일괄 반영하고 싶을 때
    

### **인터페이스를 선택해야 할 때**

- 상속 관계가 서로 다른 다양한 클래스에 **동일한 기능/행동을 부여**하고 싶을 때 (예: `Bird`, `Drone` $\rightarrow$ `IFlyable`)    
- **다중 상속** 형태의 기능 조합이 필요할 때
- 의존성 주입(Dependency Injection)이나 단위 테스트(Mocking)를 위해 객체 간의 결합도를 낮추고 싶을 때 (가장 일반적인 C# 설계 패턴)