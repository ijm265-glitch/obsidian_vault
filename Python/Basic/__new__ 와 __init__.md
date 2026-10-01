```python
class Char_Gen():
    def __init__(self, hp, mp, atk, agi):
        self.hp = hp
        self.mp = mp
        self.atk = atk
        self.agi = agi

char1 = Char_Gen(1, 2, 3, 4)
# char1 = type.__call__(Char_Gen, 1, 2, 3, 4)


# Peusdo Code
class type():
    def __call__(cls, *args, **kwargs):
        instance = cls.__new__(cls, *args, **kwargs) # Memory Allocation
        # MRO에 따라 object class의 __new__()를 사용
        # instance = object.__new__(cls, *args, **kwargs)

        if isinstance(instance, cls): # 클래스의 인스턴스가 생성되었으면
            cls.__init__(instance, *args, **kwargs) 
            # 클래스의 초기화자를 호출, 인스턴스를 인자로 넣어 속성 설정
            

        return instance # 메모리를 할당받고 속성 설정이 완료된 객체를 반환
```
- **호출 시작**: `Character_generator(1, 2, 3, 4)`를 호출한다.

- **메타클래스 개입**: `Character_generator`는 `type`의 인스턴스이므로, 파이썬은 `type.__call__(Character_generator, 1, 2, 3, 4)`를 실행한다.

- **`__new__` 탐색 및 실행**:
    - `type.__call__` 내부에서 `cls.__new__(cls, *args, **kwargs)`를 실행한다.
    - `Character_generator`에는 커스텀 `__new__`가 없으므로 MRO를 따라 부모인 `object.__new__(Character_generator)`가 호출되어 빈 인스턴스 메모리가 할당·반환된다.
    
- **`__init__` 실행**:
    - 반환된 객체가 `Character_generator`의 인스턴스가 맞으므로, `Character_generator.__init__(instance, 1, 2, 3, 4)`가 실행되어 속성(`hp`, `mp` 등)이 채워진다.
        
- **반환**: 초기화가 끝난 `instance`가 최종 반환된다.

---

Note: `.`을 사용해서 호출하면 MRO를 따른다
`Character_generator()`처럼 괄호 연산자를 사용하면 인터프리터가 즉각적으로 `type.__call__(Characcter_generator)`를 이용한다.

그러나 `Character_generator.__new__(...)`과 같이 `.`을 사용하면 일반적인 MRO를 따른다.

