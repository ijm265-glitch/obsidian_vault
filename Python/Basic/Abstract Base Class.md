- 자식 클래스가 반드시 구현해야 하는 메서드를 나타내기 위한 추상 클래스를 만들 수 있음
```python
from abc import ABC, abstractmethod

class Animal(ABC):
	def __init__(self, name: str):
		self.name = name
		
	def eat(self):
		print(f"{self.name}이 밥을 먹습니다")
		
	@abstractmethod # 자식 클래스가 구현해야 할 메서드에 데코레이터를 사용
	def make_sound(self):
		...
		
class Dog(Animal):

	def make_sound(self):
		print("멍멍")
		
# animal = Animal("동물") // ABC는 인스턴스를 만들 수 없음
dog = Dog("바둑이")
dog.make_sound()
dog.eat()
```
