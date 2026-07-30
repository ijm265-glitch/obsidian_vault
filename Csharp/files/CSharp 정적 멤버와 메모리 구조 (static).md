## 1. 정적 멤버 (Static Member)의 개념

- **인스턴스 멤버**: `new`를 통해 객체가 생성될 때마다 메모리에 각각 새로 만들어짐.
- **정적 멤버 (`static`)**: 객체를 몇 개 만들든(혹은 만들지 않든) **프로그램 실행 시 단 하나만 생성되어 클래스 전체가 공유**함.
    
```cs
public class Monster
{
    // 정적 변수: 생성된 모든 몬스터 개체 수를 공유
    public static int TotalCount = 0; 

    // 인스턴스 변수: 각 몬스터마다 개별로 갖는 HP
    public int Hp = 100;

    public Monster()
    {
        TotalCount++; // 몬스터가 new 될 때마다 공유 카운터 증가
    }
}

Console.WriteLine(Monster.TotalCount); // 인스턴스를 만들지 않아도 존재
Monster m1 = new Monster();
Monster m2 = new Monster();

Console.WriteLine(Monster.TotalCount); // 🖥️ 2 (클래스 이름으로 직접 접근!)
// Console.WriteLine(m1.TotalCount);  // 💥 에러! 인스턴스로 접근 불가
```
## 2. 정적 클래스 (`static class`)

클래스 자체에 `static`을 붙이면 **모든 멤버가 `static`이어야만 하는 클래스**가 됩니다.
- `new` 키워드로 인스턴스화할 수 없습니다.    
- 유틸리티/헬퍼 함수 모음(예: `System.Math`, `System.Console`)을 만들 때 주로 사용합니다.

```cs
public static class MathUtil
{
    // 정적 클래스 안의 모든 멤버는 static이어야 함
    public static int Add(int a, int b) => a + b;
}

// MathUtil util = new MathUtil(); // 💥 에러! 인스턴스 생성 불가
int sum = MathUtil.Add(10, 20);    // ⭕ 바로 사용
```
## 3. 정적 생성자 (Static Constructor)

정적 필드를 초기화하거나, 클래스가 처음 참조될 때 **단 한 번만 실행되는 특수한 생성자**입니다.
```cs
public class GameConfig
{
    public static readonly string ServerIp;

    // 정적 생성자 (접근 제한자 public/private 및 매개변수 사용 불가!)
    static GameConfig()
    {
        ServerIp = "127.0.0.1"; // 복잡한 정적 초기화 로직 수행
        Console.WriteLine("GameConfig 정적 생성자 실행!");
    }
}
```

- 호출 시점: 클래스의 정적 멤버에 처음 접근하거나 인스턴스가 처음 생성되기 직전에 **CLR(Common Language Runtime)에 의해 자동으로 1회만 호출**됩니다.