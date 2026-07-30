```python
from fastapi import FastAPI

# FastAPI 앱 인스턴스 (서버 프로그램의 본체) 생성
app = FastAPI()

# 누군가 브라우저에서 주소창 끝에 아무것도 안 붙이고("/") 
# 접속(GET)하면 이 함수를 실행하라는 데코레이터입니다.
@app.get("/")
def read_root() -> dict[str, str]:
    return {"Hello": "World"}
```
코드를 작성하고 아래의 명령어를 터미널에서 실행하면 서버가 시작된다.
```uvicorn main:app --reload```
```http://127.000.000.1:8000으로 접속하면 서버는 {"Hello": "World"}를 반환한다.```
# URL로 데이터 넘기기 : Path vs Query
#파라미터처리
#경로
#쿼리

|**구분**|**형태**|**주 목적 (REST API 기준)**|**필수 여부**|
|---|---|---|---|
|**경로 (Path)**|`/users/123`|**특정 자원을 식별**할 때 (예: 123번 유저)|필수|
|**쿼리 (Query)**|`/users?age=20`|자원을 **필터링, 정렬, 페이징**할 때|주로 선택적(Optional)|
```python
from fastapi import FastAPI

app = FastAPI()

# 1. 경로 매개변수 (Path Parameter)
# URL 경로에 중괄호 {} 를 사용하여 변수 자리를 만듭니다.
@app.get("/users/{user_id}")
def get_user(user_id: int):
	return {"user_id": user_id, "msg": "특정 유저 조회"}

# 2. 쿼리 매개변수 (Query Parameter)
# 경로(URL)에 {}가 없는데 함수 매개변수에 값이 있다면, 알아서 쿼리로 인식합니다.
@app.get("/items/")
def get_items(skip: int = 0, limit: int = 10):
	return {"skip": skip, "limit": limit, "msg": "목록 조회 및 페이징"}
```
# Request Body & Pydantic
#데이터검증
#유효성검사
```python
from fastapi import FastAPI
from pydantic import BaseModel

app = FastAPI()

class UserCreate(BaseModel):
	username: str
	password: str
	age: int
	bibo: str | None = None

@app.post("/users/")
def create_user(user: UserCreate):
	return {
	"msg" : f"{user.username}님 가입을 환영합니다.",
	"is_adult" : user.age >= 18
}
```
