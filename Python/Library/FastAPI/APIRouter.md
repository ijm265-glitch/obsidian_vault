`main.py`에 모든 API를 넣을 수 없기 때문에 APIRouter를 사용
일종의 미니 FastAPI로 기능별로 파일을 나누어 라우터를 만들고 `main.py`에서 조립하여 사용

`/practice/routers/users.py`
```python
from fastapi import APIRouter

# prefix를 설정하면 이 라우터의 모든 엔드포인트 주소 앞에 자동으로 /users가 붙습니다.
router = APIRouter(prefix="/users/", tags=["Users"])

@router.get("/")
def get_uesrs():
	return {"msg" : "유저 목록입니다."}
```
`/practice/main.py`
```python 
from fastapi import APIRouter
from routers import users

app = FastAPI()
app.include_router(users.router)
```