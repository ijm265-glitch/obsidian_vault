## 1. 메서드 오버로딩 (Method Overloading)

**메서드 오버로딩**은 **같은 클래스 내에서 이름은 같지만 매개변수(개수, 타입, 순서)가 다른 메서드를 여러 개 정의**하는 기능입니다.

> **핵심 규칙**
> 
> - 메서드 이름이 동일해야 합니다.
> - **매개변수의 타입, 개수, 순서 중 하나 이상이 달라야 합니다.**
> - 반환 타입(Return Type)만 다른 것은 오버로딩으로 인정되지 않습니다. (컴파일 에러 발생)
```cs
public static class Calculator
{
	public static int Add(int a, int b) => a + b;
	public static int Add(int a, int b, int c) => a + b + c;
}

Console.WriteLine(Calculator.Add(1, 2));
Console.WriteLine(Calculator.Add(1, 2, 3));

```
## 2. 상속과 `base` 키워드

상속 관계에서 **`base` 키워드**는 자식 클래스(Derived Class)가 **부모 클래스(Base Class)의 멤버(생성자, 메서드, 속성)에 접근**할 때 사용합니다.
주로 다음 두 가지 상황에서 자주 쓰입니다:
1. **부모 클래스의 생성자를 호출할 때**
```cs
public class Animal
{
	public string Name {get; set;}
	
	public Animal(string name)
	{
		Name = name;
		Console.WriteLine($"Animal Created: {Name}");
	}
}

public class Dog : Animal
{
	public string Breed {get; set;}
	
	public Dog(string name, string breed) : base(name)
	{
		Breed = breed;
		Console.WriteLine($"Breed: {Breed}");
	}
}

Dog jindo = new Dog("HASSAN", "JINDO");
```
**2. 부모 클래스의 메서드(오버라이딩 전 원본 메서드)를 호출할 때**
```cs
public class Animal
{
	public float weight {get; set;}
	public float height {get; set;}
	
	public Animal(float Weight, float Height)
	{
		weight = Weight;
		height = Height;
	}
	
	public virtual void Walk()
	{
		Console.WriteLine("I can walk!");
	}
}

public class Bird : Animal
{
	public bool is_flyable {get; set;}
	
	public Bird(float Weight, float Height, bool is_flyable) : base(Weight, Height)
	{
		this.is_flyable = is_flyable;
	}
	
	public override void Walk()
	{
		base.Walk();
		Console.WriteLine("That might be hopping...");
	}
	
	public void Fly()
	{
		if (is_flyable != true)
		{
			Console.WriteLine("You Can't Fly");
			return;
		}
		
		base.Walk();
		Console.WriteLine("and I can fly!!");
	}
	
	public void info()
	{
		Console.WriteLine($"Weight: {weight}");
		Console.WriteLine($"Height: {height}");
		Console.WriteLine($"is_flyable: {is_flyable}");
	}
}
Animal a1 = new Animal(175.0f, 75.3f);
a1.Walk();
Bird b1 = new Bird(175.0f, 75.3f, true);
b1.info();
b1.Walk();
b1.Fly();

```