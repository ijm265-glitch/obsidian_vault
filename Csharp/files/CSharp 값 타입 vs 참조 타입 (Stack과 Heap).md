| **구분**       | **값 타입 (Value Type)**                            | **참조 타입 (Reference Type)**                           |
| ------------ | ------------------------------------------------ | ---------------------------------------------------- |
| **저장 위치**    | **스택(Stack)** 영역                                 | **힙(Heap)** 영역 (주소값은 Stack)                          |
| **변수 저장 내용** | 데이터 **실제 값** 자체                                  | 데이터가 저장된 **힙 메모리 주소(참조)**                            |
| **복사 방식**    | **값 복사** (Deep Copy 형태)                          | **주소 복사** (Shallow Copy 형태)                          |
| **메모리 해제**   | 스코프(`{ }`)를 벗어나면 **즉시 해제**                       | **가비지 컬렉터(GC)**가 비동기로 해제                             |
| **대표 종류**    | `int`, `float`, `bool`, `char`, `struct`, `enum` | `class`, `string`, `array([]),` `delegate`, `object` |

**값 타입의 작동방식**
```Cs
int x = 10;
int y = x;

y = 20;

Console.WriteLine(x); // 출력: 10
Console.WriteLine(y); // 출력: 20
```

**참조 타입의 작동방식**
```Cs
class Student
{
	public string Name;
}

Student s1 = new Student();
s1.Name = "홍길동";

Student s2 = s1; // s1객체의 주소를 복사함
s2.Name = "이순신";

Console.WriteLine(s1.Name); // 출력: 이순신 (s1의 값이 바뀜)
Console.WriteLine(s2.Name); // 출력: 이순신
```

**문자열**
- 참조 타입이지만 immutable이기 때문에 값을 변경하려고 하면 새로운 문자열 개개체를 새로 생성함 (따라서 값 타입처럼 동작함)
```Cs
string str1 = "Hello";
string str2 = str1;

str2 = "World";

Console.WriteLine(str1); // 출력: Hello (안 바뀜) Console.WriteLine(str2); // 출력: World (새로운 객체가 힙에 생성됨)


```