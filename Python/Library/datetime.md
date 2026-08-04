## 1. 현재 날짜 및 시간 구하기 (`datetime` 클래스)
```python
from datetime import datetime

# 1. 현재 날짜와 시간 구하기 (Local time)
now = datetime.now()
print(now)  # 예: 2026-08-03 10:15:30.123456

# 2. 오늘 날짜 구하기 (시/분/초 제외)
today = datetime.today()

# 3. 특정 날짜/시간 객체 직접 생성하기
# (연, 월, 일, 시, 분, 초)
dt = datetime(2026, 8, 3, 14, 30, 0)
print(dt)  # 2026-08-03 14:30:00

# 4. 개별 요소(년, 월, 일, 시, 분, 초) 접근하기
print(now.year)    # 년도 (2026)
print(now.month)   # 월 (8)
print(now.day)     # 일 (3)
print(now.hour)    # 시 (10)
print(now.minute)  # 분 (15)
print(now.second)  # 초 (30)
```

## 2. 문자열 ↔ datetime 변환 (가장 중요한 함수)

카메라 파일명이나 로그 메시지를 다룰 때 필수적인 두 함수입니다.

### ① `strftime()`: datetime ➔ 문자열 (Formatting)

시간 객체를 지정한 서식의 **문자열로 변환**합니다.
```Python
now = datetime.now()

# 2026-08-03 10:15:30
str1 = now.strftime("%Y-%m-%d %H:%M:%S")

# 20260803_101530 (파일명으로 자주 쓰임)
str2 = now.strftime("%Y%m%d_%H%M%S")
```

### ② `strptime()`: 문자열 ➔ datetime (Parsing)

문자열로 된 날짜 데이터를 **datetime 객체로 변환**합니다.
```Python
date_str = "2026-08-03 14:30:00"

# 문자열의 형태와 서식 포맷이 정확히 일치해야 합니다.
parsed_dt = datetime.strptime(date_str, "%Y-%m-%d %H:%M:%S")
print(parsed_dt.year)  # 2026
```

> **자주 쓰이는 서식 기호**
> 
> - `%Y`: 4자리 연도 (2026) / `%y`: 2자리 연도 (26)
>     
> - `%m`: 2자리 월 (01~12)
>     
> - `%d`: 2자리 일 (01~31)
>     
> - `%H`: 24시간제 시 (00~23) / `%I`: 12시간제 시 (01~12)
>     
> - `%M`: 2자리 분 (00~59)
>     
> - `%S`: 2자리 초 (00~59)
>     
> - `%A`: 요일 이름 (Monday, Tuesday...)


## 3. 날짜/시간 연산하기 (`timedelta` 클래스)

날짜에 **며칠을 더하거나 빼야 할 때**, 또는 **시간 차이를 계산할 때** 사용합니다.

```Python
from datetime import datetime, timedelta

now = datetime.now()

# 1. 특정 시간 더하고 빼기
three_days_later = now + timedelta(days=3)       # 3일 뒤
one_hour_ago = now - timedelta(hours=1)          # 1시간 전
ten_minutes_later = now + timedelta(minutes=10)   # 10분 뒤

print("3일 뒤:", three_days_later)

# 2. 두 날짜/시간의 차이 계산하기
d1 = datetime(2026, 8, 3)
d2 = datetime(2026, 8, 10)

diff = d2 - d1
print(diff.days)  # 7 (일 수 차이)
```

## 4. 단순 날짜 / 시간만 다루기 (`date`, `time` 클래스)

시간 없이 **'날짜'만** 필요하거나, 날짜 없이 **'시/분/초'만** 필요할 때 사용합니다.
```Python
from datetime import date, time

# 날짜만 다루기
d = date(2026, 8, 3)
print(d)  # 2026-08-03

# 시간만 다루기 (시, 분, 초)
t = time(14, 30, 0)
print(t)  # 14:30:00

# 요일 구하기 (weekday)
# 0: 월요일, 1: 화요일 ... 6: 일요일
print(d.weekday())
```