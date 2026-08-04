# generator
```python
def generator():
	print("generator 실행됨")
	yield print("yield 실행됨")
	print("generator 종료됨")
	
	
gen = generator() # generator를 실행하지 않고 힙 영역에 generator객체를 만들어 gen이라는 이름으로 이름표를 붙임 - 콜 스택에 올라가지 않음
next(gen) 
'''
힙에 대기중인 객체가 콜스택으로 올라가서 실행되고 yield를 실행하면 지금까지의 변수값과 현재 멈춘 줄 번호(Instrution Pointer)를 객체에 저장하여 다시 힙으로 이동 - print("generator 실행됨")
yield print("yield 실행됨")
'''
next(gen) # 힙에 대기중인 객체를 다시 콜스택으로 올려 실행한다. 이전에 실행했던 정보인 변수값과 줄번호를 반영하여 그 다음부터 실행을 이어간다. - print("generator 종료됨"), raise StopIteration

```

# asyncio
## awaitable
1. Coroutine : `await make_coffee()`비동기 함수를 직접 호출하며 대기
2. Task : `asyncio.create_task()`로 백그라운드에 던져둔 작업표 대기
3. 특수 객체 : (Future, asyncio.sleep)
### await
- `await coroutine` : coroutine을 실행하고(coroutine에게 스레드 제어권을 넘기고) coroutine이 pending상태가 되어 제어권이 이 await를 실행한 함수에게 돌아오면 이 스레드 제어권을 이 await를 실
행한 함수를 호출한 함수에게로 넘김 
- `await task` : task가 완료될 때 까지 pending상태가 되어 스레드 제어권을 이 await를 실행한 함수를 호출한 함수에게로 넘김, task가 완전히 완료되면 await task로 다시 스레드 제어권을 줌
```python 
async def coroutine():
	print("코루틴 생성됨")
	
obj = coroutine() # 클래스를 호출하면 객체를 생성하듯 코루틴을 호출하면 코루틴 객체를 힙에 생성
await obj # 코루틴 객체를 콜 스택으로 가져와 실제로 실행

await coroutine() # 코루틴 객체를 힙에 만들어서 콜 스택에 올리고 바로 실행
```
## 기본 흐름
```python
import asyncio

async def A():
	await B()
	
async def B():
	await async.sleep(10)
	
async.run(A())
```
1. `async.run(A())` : 힙에 A 코루틴 객체를 생성하고 이벤트 루프를 생성해서 A 객체를 콜스택에 올림
2. `await B()` : 힙에 B 코루틴 객체를 생성하고 await를 통해 실행함 - B 객체를 추가적으로 콜스택에 올림
3. `await async.sleep(10)` :  I/O Bound작업을 조우하면 B가 Pending상태로 전환되고 콜스택에서 분리되어 힙으로 이동
4. B가 스레드를 반납하면 A역시 대기 상태이므로 스택을 타고 내려가 최종적으로 이벤트 루프에게 반환
5. 제어권을 반납받은 이벤트 루프가 준비 큐를 확인하고 실행할 테스크가 없으면 대기
6. 지정된 시간이 경과하며 OS의 타이머 인터럽트가 발생하면 I/O작업이 완료되었음을 알리는 완료 콜백 객체가 이벤트 루프의 준비 큐에 삽입됨
7. 이벤트 루프가 준비큐를 확인하고 힙에 보존되어 있던 A와 B 프레임 객체를 콜스택에 적재하여 코루틴의 실행을 재개
## asyncio.create_task()
```python
import asyncio

async def background_worker():
	print(" [직원] 백그라운드 작업 시작!")
	await asyncio.sleep(2)
	print(" [직원] 백그라운드 작업 완료!")

async def main():
	print("[메인] 직원 채용 중...")
	task = asyncio.create_task(background_worker()) 
	print("[메인] 직원이 일하는 동안 메인은 다른 일(딴짓)을 합니다.")
	await asyncio.sleep(1) 
	
	print("[메인] 이제 직원이 일 다 할 때까지 기다려볼까?")
	await task # 직원이 끝날 때까지 대기
	
	print("[메인] 모든 작업 종료!")

asyncio.run(main())
```
1. `asyncio.run(main())` : 힙에 main 코루틴 객체를 생성하고 이벤트 루프를 생성해서 main 객체를 콜스택에 올림
2. `task = asyncio.create_task(background_worker())` background_worker객체를 힙에 생성하고 Task객체로 캡슐화한다.
3. Task객체로 만들고 이벤트 루프에서 call_soon()이라는 메서드를 자동적으로 호출한다
4. call_soon() 메서드는 이벤트 루프 내부의 `_ready` 라는 이름의 deque에  이 태스트의 실행을 예약한다. 
5. `await asyncio.sleep(1) ` : main이 자신의 스레드 제어권을 이벤트 루프에 반환하고 이벤트 루프는 ready queue에 있는 task를 실행한다.
6. `await asyncio.sleep(2)` : task는 자신의 스레드 제어권을 다시 이벤트 루프에 반환한다.
7. 1초가 지나 main의 타이머 인터럽트가 발생하여 완료 콜백 객체가 이벤트 루프의 준비큐에 삽입된다.
8. 이벤트 루프는 이를 확인하고 main함수를 콜스택으로 가져와 이어서 실행한다.
9. `await task` : task의 작업이 완료될 추가적인 1초동안 main의 스레드 제어권을 이벤트루프에 반환한다.
10. 1초가 지나 준비 큐에 완료 콜백 객체가 삽입되고 이를 확인한 이벤트 루프가 콜스택에 task를 올려 이어서 실행한다. 
11. `main`의 완료 콜백 객체를 준비 큐(`_ready` deque)에 다시 넣는다
12. 프로그램 종료
## asyncio.gather
```python
import asyncio

async def process_audio_chunk(chunk_id):
	print(f"오디오 {chunk_id}번 조각 처리 시작")
	await asyncio.sleep(2) # (I/O 대기)
	return f"{chunk_id}번 텍스트 결과"

async def main():
	print("=== 5개 조각 동시 처리 시작 ===")
	
	results = await asyncio.gather(
		process_audio_chunk(1),
		process_audio_chunk(2),
		process_audio_chunk(3),
		process_audio_chunk(4),
		process_audio_chunk(5)
	)
	print("=== 모든 처리 완료 ===")
	print("합쳐진 결과:", results)

asyncio.run(main())
```
1. `asyncio.run(main())` : 힙에 main 코루틴 객체를 생성하고 이벤트 루프를 생성해서 main 객체를 콜스택에 올림
`results = await asyncio.gather(process_audio_chunk(1), process_audio_chunk(2), process_audio_chunk(3), process_audio_chunk(4), process_audio_chunk(5))`
2. `process_audio_chunk(1)` : 5개의 코루틴 객체를 힙에 생성한다. 
3. `asyncio.gather(...)` : 5개의 코루틴 객체를 Task객체로 캡슐화 한 뒤 이벤트 루프의 ready queue에 삽입한다
4. `await`  : `asyncio.gather(...)`을 실행하고 스레드 제어권을 돌려받았으모로`main`함수가 스레드 제어권을 이벤트 루프로 반납함
5.  이벤트 루프가 대기열을 확인하고 `process_audio_chunk(1)`객체를 콜스택으로 올려서 실행
6. `await asyncio.sleep(2)`를 실행하며 pending상태가 되고 제어권을 다시 이벤트 루프로 반납
7. 이벤트 루프가 대기열을 확인하고 `process_audio_chunk(2)`객체를 콜스택으로 올려서 실행
8. `await asyncio.sleep(2)`를 실행하며 pending상태가 되고 제어권을 다시 이벤트 루프로 반납을 반복
9. 5개의 객체가 pending상태에 있다가 약 2초가 경과하면 5개 Task의 완료 콜백이 준비큐에 다시 삽입됨
10.  이벤트 루프는 준비큐를 확인하고 이를 다시 콜스택에 올려 이어서 실행
11.  전부 실행이 완료되면 gather는 배열에 넣은 순서대로 조합하여 리스트 생성
12. `gather`객체가 `main`의 콜백을 다시 준비큐에 넣어 콜스택에서 실행
13. 프로그램 종료

## task.cancel(), CancelledError
```python
import asyncio

async def risky_worker():

	try:
		print("[직원] 10초짜리 작업 시작합니다...")
		await asyncio.sleep(10))
		print("[직원] 작업 완료!") # 취소되면 이 줄은 실행되지 않음

	except asyncio.CancelledError:
		print("[직원] 앗! 취소 명령이 들어왔습니다! 사용 중인 메모리를 비우고 퇴근합니다.")
		raise # (선택) 취소되었다는 사실을 호출자에게도 알리기 위해 다시 에러를 던짐

async def main():
	task = asyncio.create_task(risky_worker())
	
	await asyncio.sleep(2) # 2초 기다려보고...
	
	print("[메인] 2초 지났는데 안 끝나? 당장 취소해!")
	task.cancel() # 마이크 회수!

	try:
		await task # 취소된 태스크를 await 하면 메인에서도 CancelledError가 터집니다.
	except asyncio.CancelledError:
		print("[메인] 직원이 성공적으로 작업을 취소하고 퇴근했습니다.")

asyncio.run(main())
```

## asyncio에서 비동기 함수 사용
```python
import asyncio
import requests
import concurrent.futures

def blocking_request(url):
	# 이 녀석은 멈춰버리는 동기 함수입니다! (Blocking)
	return requests.get(url).status_code

async def main():
	loop = asyncio.get_running_loop()
	executor = concurrent.futures.ThreadPoolExecutor(max_workers=3)
	print(type(executor))
	print("비동기 루프는 멈추지 않고 계속 다른 일을 합니다.")
	# asyncio.to_thread()
	task = loop.run_in_executor(executor, blocking_request, 
	"https://www.google.com")
	# 강제로 컨텍스트 스위칭을 하게 해서 멈추지 않고 작동하도록 함
	
	print("요청은 보냈지만, 루프는 즉시 다음 줄로 넘어왔습니다!")
	status = await task
	print(f"응답 결과: {status}")
	
asyncio.run(main())
```