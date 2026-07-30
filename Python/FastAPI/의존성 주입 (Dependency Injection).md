의존성 : API가 돌아가기 위해 필요한 부품 (DB 연결, 로그인 검증 등)
주입 : API 안에서 직접 부품을 만들지 않고, Depends를 이용해 밖에서 완성된 부품을 넣어주는 행위
서버가 요청을 받으면
- `Depends(function)` : function을 실행하고 그 반환값을 가져옴
- `Depends(generator)`  : generator를 호출하여 힙에 생성하고 `__next__()`메서드를 실행하여 그 반환값을 가져옴

## 1. 공통 로직 재사용
```python
from fastapi import FastAPI, Depends

app = FastAPI()

# 의존성 함수 정의
def com_params(skip: int = 0, limit: int = 100): 
# 목록을 불러오는 API의 건더뛸 개수와 가져올 개수
	return {"skip" : skip, "limit" : limit}

@app.get("/posts/")
def read_posts(commons: dict = Depends(com_params)):
	return {"msg" : "게시글 목록", "params" : commons}

# GET /posts/가 들어오면 먼저 com_params함수를 실행하고
# 그 반환값을 commons 변수에 넣어 비즈니스 로직을 실행
```
## 2. 데이터베이스 세션 관리
- 데이터 베이스는 연결 - 작업 - 닫기의 과정을 거침
- 에러가 발생하여 닫기의 과정을 실행하지 못하면 서버가 다운될 수 있음
- `fastapi.Depends`와 `yield`를 사용하여 관리를 자동화 할 수 있음
```python
from fastapi import FastAPI, Depends

app = FastAPI()

# 가상의 데이터베이스 세션 객체
class SessionLocal:
	def close(self):
		print("DB 연결 종료")

# 의존성 함수 : DB 세션 생성 및 안전한 종료 보장
def get_db():
	db = SessionLocal() # DB 연결 열기
	try:
		yield db # API 엔드포인트에 DB 객체를 주입
	finally:
		db.close # API 처리가 끝나면 무조건 실행
		
@app.get("/users/")
def read_users(db=Depends(get_db)):
	return {"msg" : "유저 데이터 조회 성공"}
''' Depends가 get_db 제너레이터를 호출하여 생성하고 next()를 실행하여 값을 반환받아
parameter인 db의 인자로 넣어줌 '''
```
## 3. 인증 및 권한 확인
- 특정 API는 로그인한 유저 또는 관리자에게만 접근되어야 함 
- 이를 위해 의존성 함수를 구성할 수 있음
```python
from fastapi import FastAPI, Depends, HTTPException, Header

app = FastAPI()

# 의존성 함수: 헤더의 토큰 검사
def verify_token(x_token: str = Header(...)): # Header(...) : 생략할 수 없는 헤더값
	if x_token != "super-secret-token":
		raise HTTPException(status_code=400, detail="유효하지 않은 토큰입니다.")
	return x_token

@app.get("/vip-data/")
def get_vip_data(token: str = Depends(verify_token), accept: str = Header(None)): # 생략할 수 있는 헤더값
	if accept == "video/mp4":
		raise HTTPException(status_code=406, detail="동영상 제공 불가")
	return {"msg": "VIP 전용 데이터입니다.", "used_token": token}
```
`httpx`로 헤더를 설정하여 get요청 전송
```python
import httpx

# 1. 요청을 보낼 주소
url = "http://localhost:8000/vip-data/"
# 2. 택배 겉면에 붙일 송장(헤더) 만들기
# 주의: 언더바(x_token)가 아니라 하이픈(x-token)을 써야 합니다!
my_headers = {
"X-Token": "super-secret-token",
"accept" : "video/mp4"
}
# 3. GET 요청을 보낼 때 headers 옵션에 송장 스티커 붙이기
response = httpx.get(url, headers=my_headers)

# 4. 결과 확인
print("상태 코드:", response.status_code)
print("서버 응답 내용:", response.json())
print("내가 진짜로 보낸 전체 헤더:", response.request.headers)
```
