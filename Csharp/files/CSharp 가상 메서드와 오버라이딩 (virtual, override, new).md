## 1. virtual, override, new 세 키워드의 역할

- **`virtual` (부모 클래스)**: "이 메서드는 자식 클래스에서 재정의(override)하는 것을 허용하겠다."
- **`override` (자식 클래스)**: "부모의 `virtual` 메서드를 완전히 대체(재정의)하겠다."
- **`new` (자식 클래스)**: "부모의 메서드와 이름만 같을 뿐, 완전히 별개의 **새로운 메서드로 부모 메서드를 가리겠다(Hide).**"
```cs
public class Parent
{
	public void say_hello() => Console.WriteLine("Hello");
	public virtual void eat() => Console.WriteLine("Eating Vegetable");
	public void walk() => Console.WriteLine("Walking...");
}

public class Child : Parent
{
	public override void eat() => Console.WriteLine("Eating Meat!");
	public new void walk() => Console.WriteLine("Fuck you, I don't want to walk");
}

// new, override둘 다 부모의 메서드를 실행하지 않고 자식 클래스의 메서드로 실행
Child c1 = new Child();
c1.say_hello();
c1.eat();
c1.walk();

// Upcasting을 하면 override한 메서드는 자식 클래스에서 정의한 것으로 완전히 바뀌었고 new는 부모 클래스 그대로 유지됨
Parent parentRef = new Child();
parentRef.eat();
parentRef.walk();
```
### 핵심적인 차이
### 1) `override`의 동작 (동적 바인딩)

`parentRef`는 타입이 `Parent`이지만, 실행할 때 실제 메모리에 있는 객체(`Child`)를 찾아가서 [[vtable]](가상 메서드 테이블)을 확인합니다. `override`되었기 때문에 **진짜 알맹이인 자식의 `MethodA`를 실행**합니다.
> 💡 **다형성의 핵심:** `List<Parent>`에 다양한 자식 객체들을 넣어 두고 한 번에 `MethodA()`를 호출해도 각각 자식 객체의 메서드가 알아서 동작합니다.
### 2) `new`의 동작 (메서드 숨김 / 정적 바인딩)
`new`는 상속 관계를 끊고 **이름만 같은 새 메서드**를 만든 것입니다. 컴파일러는 `parentRef`의 변수 타입(`Parent`)만 보고 **부모의 `MethodB`를 그대로 호출**해버립니다. 자식의 `MethodB`는 `Parent` 타입 입장에서는 존재조차 모르는 상태가 됩니다.