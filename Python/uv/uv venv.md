## 1. 가상환경 생성

- `.venv`라는 가상환경 폴더 생성
```python
uv venv
```

- 특정 Python 버전 지정 생성
```python
uv venv --python 3.11
```

- 원하는 폴더 이름으로 생성
```python
uv venv myenv
```

## 2. 가상환경 활성화하기

사용 중인 OS 및 Shell 환경에 맞는 명령어를 실행합니다.

|**운영체제 / Shell**|**활성화 명령어**|
|---|---|
|**Windows (PowerShell)**|`.venv\Scripts\Activate.ps1`|
|**Windows (CMD)**|`.venv\Scripts\activate.bat`|
|**macOS / Linux (Bash, Zsh)**|`source .venv/bin/activate`|
