클래스 이름 뒤에 `<T>`를 붙여 정의하며, 필요한 경우 여러 개의 타입 매개변수(`<TKey, TValue>`)를 사용할 수 있습니다.
### 제네릭 클래스
```cs
public class CustomBuffer<T>
{
	private T[] _buffer;
	private int _index = 0;
	
	public CustomBuffer(int capacity)
	{
		_buffer = new T[capacity];
	}
	
	public void Add(T item)
	{
		if (_index < _buffer.Length)
		{
			_buffer[_index++] = item;
		}
	}
	
	public T Get(int index) => _buffer[index];
}

CustomBuffer<string> alp = new CustomBuffer<string>(10);
alp.Add("a");
alp.Add("b");
alp.Add("c");
alp.Add("d");
alp.Add("e");
Console.WriteLine(alp.Get(3));
```

### 제네릭 메서드
```cs
public class Utility
{
	public static void Swap<T>(ref T a, ref T b)
	{
		T temp = a;
		a = b;
		b = temp;
	}
}

int a = 10, b = 20;
Utility.Swap<int>(ref a, ref b);
Console.WriteLine($"a: {a}, b: {b}");

```

### 제네릭 제약 조건 (Generic Constraints: `where`)

`T`에 아무 타입이나 다 들어오면 클래스 내부에서 특정 메서드나 속성을 호출할 수 없습니다. **`where` 키워드를 사용해 `T`가 들어올 수 있는 타입을 제한**할 수 있습니다.
### 🔑 자주 쓰이는 제약 조건

| **제약 조건**                      | **설명**                              |
| ------------------------------ | ----------------------------------- |
| **`where T : struct`**         | `T`는 반드시 **값 타입(구조체)**이어야 함         |
| **`where T : class`**          | `T`는 반드시 **참조 타입(클래스)**이어야 함        |
| **`where T : new()`**          | `T`는 반드시 **매개변수가 없는 기본 생성자**를 가져야 함 |
| **`where T : BaseClass`**      | `T`는 반드시 지정한 **부모 클래스를 상속**받아야 함    |
| **`where T : ISomeInterface`** | `T`는 반드시 지정한 **인터페이스를 구현**해야 함      |

```cs
// 💻 제약 조건 활용 예시 (게임 엔티티 매니저)
public interface IEntity
{
    int Id { get; get; }
}

public class EntityManager<T> where T : class, IEntity, new()
{
    private Dictionary<int, T> _entities = new Dictionary<int, T>();

    public T CreateEntity(int id)
    {
        // where T : new() 제약이 있으므로 new T() 가능
        T newEntity = new T(); 
        
        // where T : IEntity 제약이 있으므로 .Id 접근 가능
        newEntity.Id = id; 
        
        _entities.Add(id, newEntity);
        return newEntity;
    }
}
```