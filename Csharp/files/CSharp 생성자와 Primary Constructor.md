## 1. 클래식 생성자 (Classic Constructor)

클래스 이름과 동일한 이름을 가진 반환 타입이 없는 메서드로 작성하며, 필요에 따라 매개변수를 달리하는 오버로딩(Overloading)이 가능합니다.

```cs
public class Player
{
	private string _name;
	private int _level;
	
	// Constructor
	public Player()
	{
		_name = "Guest";
		_level = 1;
	}
	
	// Constructor Overloading
	public Player(string name, int level)
	{
		this._name = name;
		this._level = level;
	}
	
	// Copy Constructor
	public Player(Player Other)
	{
		if (Other == null) return;
		
		// 값 타입 변수이기 때문에 메모리의 저장값 가져옴
		this._name = Other._name; 
		this._level = Other._level
	}
	
	// Destructor
	public ~Player()
	{
		Console.WriteLine("GoodBye World!");
	}
	
}
```


## 2. Primary Constructor
클래스/구조체 이름 바로 뒤에 매가변수 목록을 정의하여 매개변수를 직접 사용
```cs
public class Player(string name, int level)
{
	public string Name {get; set;} = name;
	public int Level {get; set;} = level;
	
	public void PrintInfo()
	{
		Console.WriteLine($"{Name} (Lv.{Level})");
	}
}

Player p1 = new Player("LJM", 26);
p1.PrintInfo();
```
## 복사생성자

**C#의 class는 reference type이기 때문에 class의 member variable이 class의 인스턴스일 경우 얕은 복사가 되어(객체의 주소만 가져옴) 문제가 될 수 있음**
```cs
class Player
{
	public string Name {get; set;}
	public Weapon EquipWeapon {get; set;}
	
	// Constructor
	public Player()
	{
		Name = "@@";
		Weapon = new Weapon();
	}
	
	// Copy Constructor
	public Player(Player other)
	{
		this.Name = other.Name;
		this.EquipWeapon = other.EquipWeapon; // ⚠️ 메모리 주소만 그대로 복사됨!    // this.EquipWeapon = 
		
	}
}
```
**메모리 상의 같은 주소를 향하고 있기 때문에 의도와 다른 동작을 함**

**해결방법**
1. `Weapon` 클래스에 복사 생성자 만들기
```cs
public class Weapon
{
	public string Name {get; set;}
	public int Damage {get; set;}
	
	public Weapon(string Name, int Damage)
	{
		Name = name;
		Damage = damage;
	}
	
	public Weapon(Weapon other)
	{
		this.Name = other.Name;
		this.Damage = other.Damage;
	}
}

public class Player
{
	public string Name {get; set;}
	public Weapon Equipweapon {get; set;}
	
	public Player(string name)
	{
		this.Name = name;
		this.Equipweapon = new Weapon("gog", 1000);
	}
	
	public Player(Player other)
	{
		this.Name = other.Name;
		this.Equipweapon = new Weapon(other.Equipweapon);
	}
}
```