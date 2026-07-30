### 1. vtable이란?
`vtalbe` (Virtual Method Table)은 클래스에 속한 virtual method의 실제 메모리 주소를 모아둔 배열임
`virtual`키워드를 사용한 메서드가 있으면 컴파일 시점에 클래스마다 메모리에 1개씩 생성됨
## 2. 메모리 내부 구조: `vptr`과 `vtable`

객체가 메모리(힙 영역)에 생성될 때, C# 컴파일러는 객체의 필드(변수) 외에 **눈에 보이지 않는 숨겨진 포인터 1개**를 객체의 맨 앞에 몰래 집어넣습니다. 이것을 `vptr`(Virtual Pointer)이라고 부릅니다.
- **`vptr` (Virtual Pointer)**: 객체마다 존재하며, 자신이 속한 클래스의 `vtable` 메모리 주소를 가리킵니다.
- **`vtable`**: 클래스마다 1개 존재하며, 가상 메서드들의 실제 실행 주소를 목록으로 가지고 있습니다.
```
[힙 메모리의 Dog 객체]             [메모리의 Dog 클래스 vtable]
+--------------------+            +-------------------------------+
|  vptr (숨겨진 포인터) | ---------> | Index 0: &Dog.MakeSound()     |
|  weight = 10       |            | Index 1: &Animal.Sleep()      |
|  height = 50       |            +-------------------------------+
+--------------------+
```

## 3. `virtual`과 `override` 시 vtable에서 일어나는 일
```cs
public class Animal
{
    public virtual void MakeSound() => Console.WriteLine("Animal Sound");
    public virtual void Sleep()     => Console.WriteLine("Animal Sleep");
}

public class Dog : Animal
{
    // MakeSound만 오버라이드함
    public override void MakeSound() => Console.WriteLine("Dog Sound");
}
```
### Step 1. `Animal` 클래스의 vtable 생성 (컴파일 시점)
`Animal` 클래스에 `virtual` 메서드가 2개 있으므로, 컴파일러는 다음과 같은 `Animal vtable`을 만듭니다.

| **Index** | **메서드 슬롯**    | **가리키는 메모리 주소**                  |
| --------- | ------------- | -------------------------------- |
| **0**     | `MakeSound()` | `&Animal.MakeSound()` (부모 원본 주소) |
| **1**     | `Sleep()`     | `&Animal.Sleep()` (부모 원본 주소)     |
### Step 2. `Dog` 클래스의 vtable 생성 (`override`의 핵심!)
`Dog`는 `Animal`을 상속받았으므로, 먼저 부모의 `vtable` 복사본을 가져옵니다.
그 후, **`override`가 선언된 메서드의 주소만 자식의 메서드 주소로 '덮어쓰기(Override)'** 합니다.

|**Index**|**메서드 슬롯**|**가리키는 메모리 주소**|**설명**|
|---|---|---|---|
|**0**|`MakeSound()`|**`&Dog.MakeSound()`**|**`override`했으므로 자식 주소로 덮어씀!**|
|**1**|`Sleep()`|`&Animal.Sleep()`|오버라이드 안 했으므로 부모 주소 유지|
## 4. 런타임에 동적 바인딩이 일어나는 과정

이제 아래 코드가 실행될 때 CPU와 메모리에서 어떤 일이 벌어지는지 추적해 봅시다.
```cs
Animal myPet = new Dog(); // 업캐스팅
myPet.MakeSound();        // 동적 바인딩 발생!
```

### 🔍 실행 과정 (CPU의 시선)

1. **타입 확인**: 컴파일러는 `myPet`이 `Animal` 타입인 것을 알고 있습니다. `Animal` 클래스의 `MakeSound`는 `vtable`의 **Index 0**번에 위치해 있다는 것을 컴파일 시점에 파악해 둡니다.
2. **실제 객체 접근**: 프로그램 실행 중에 `myPet` 변수를 따라 **실제 힙 메모리에 있는 `Dog` 객체**로 찾아갑니다.
3. **`vptr` 추적**: `Dog` 객체의 맨 앞에 숨겨진 `vptr`을 확인합니다. 이 `vptr`은 `Dog` 클래스의 `vtable`을 가리키고 있습니다.
4. **`vtable` 조회**: `Dog vtable`에서 아까 약속했던 **Index 0**번 슬롯을 열어봅니다.
5. **호출**: Index 0번에 들어있는 주소인 `&Dog.MakeSound()`를 실행합니다!
    

결과적으로 변수의 타입(`Animal`)과 상관없이, **실제 인스턴스(`Dog`)의 `vtable`을 조회했기 때문에 "Dog Sound"가 출력**됩니다.