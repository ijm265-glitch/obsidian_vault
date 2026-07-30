# 기본타입 
- 기초적인 스칼라 데이터 타입
```python
name: str = "Alice"
age: int = 30
height: float = 165.3
is_student: bool = False
```

# 컬렉션 타입
- 자료구조 내부의 데이터 타입 지정
```python
# list
scores: list[int] = [98, 50, 88]
# set
unique_names: set[str] = {'Alice', 'Bob'}
# dictonary : dict[Keytype, ValueType]
user_info: dict[str, int] = {"Alice": 30, "Bob": 25}
# tuple : 요소의 개수와 위치별 타입 명시
coordinates: tuple[float, float] = (37.5665, 126.9780)
# 가변 길이 튜플: 타입 뒤에 '...') Ellipsis 사용
versions: tuple[int, ...] = (1, 2, 3, 4, 5)
```
# 다중 타입과 옵션 (Union & Optional)
```python
# data의 타입이 int거나 str이고 함수는 str타입을 반환
def process_data(data: int | str) -> str:
	return str(data)
# 인자가 특정 타입이거나 None일 수 있음
def get_user(user_id: int, default: str | None = None) -> str:
	if default is None:
		return "Unknown"
	return default
```
# 함수와 Callable
- 함수 자체를 변수에 할당하거나 매개변수로 넘길 때 사용
- Callable\[\[인자타입1], \[인자타입2], 반환타입]
- Callable\[..., 반환타입]
```python
from typing import Callable 

def apply_operation(a: int, b: int, op: Callable[[int, int], int]) -> int:
	return op(a, b)

def timer(func: Callable[..., None]) -> Callable[..., float]:
	@wraps(func)
	def wrapper(*args: Any, **kwargs: Any) -> float:
		ts = time.time()
		func(*args, **kwargs)
		dt = time.time() - ts
		return dt
	return wrapper
```

# 추상화 및 덕 타이핑 
- Iterable, Sequence, Mapping
```python
from typing import Iterable, Sequence, Mapping 

# Iterable: for문으로 순회할 수 있는 모든 것 (list, tuple, set, str 등) 
def print_all(items: Iterable[str]) -> None: 
	for item in items: print(item) 
	
# Sequence: 순서가 있고 인덱싱(items[0])이 가능한 것 (list, tuple, str 등) 
def get_first(items: Sequence[int]) -> int: 
	return items[0] 
	
# Mapping: 키-값 쌍으로 이루어진 것 (dict 등) 
def print_keys(data: Mapping[str, int]) -> None: 
	print(data.keys())
```

# 고급 타입 제어
```python
from typing import Any, Literal, TypedDict, NoReturn

# 1. Any: 타입 검사를 포기함 (최후의 보루로만 사용)
raw_data: Any = get_data_from_legacy_api()

# 2. Literal: 특정 '값' 자체를 타입으로 지정
def move_character(direction: Literal["up", "down", "left", "right"]) -> None:
    pass

# 3. TypedDict: dict의 특정 키와 값 타입을 엄격하게 정의
class User(TypedDict):
    name: str
    age: int

user: User = {"name": "Alice", "age": 30} # 키가 빠지거나 타입이 다르면 경고

# 4. NoReturn: 함수가 정상적으로 끝나지 않음 (항상 예외 발생 또는 무한 루프)
def crash_system(msg: str) -> NoReturn:
    raise RuntimeError(msg)
```

# 제네릭 
- 타입을 변수처럼 다루어, 함수나 클래스가 다양한 타입에 대한 동일 로직을 수행하게 함
```python
from typing import TypeVar

# T라는 타입 변수 생성
T = TypeVar('T')

# 입력받은 타입(T)과 동일한 타입(T)을 반환함을 보장
def get_first_item(items: list[T]) -> T:
    return items[0]

# get_first_item([1, 2, 3]) -> int 반환으로 추론됨
# get_first_item(["a", "b"]) -> str 반환으로 추론됨

```
  