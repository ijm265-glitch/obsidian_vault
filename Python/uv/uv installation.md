## 1. `uv` 설치하기

### **Windows**

PowerShell을 열고 아래 명령어를 실행

```PowerShell
powershell -ExecutionPolicy ByPass -c "irm https://astral.sh/uv/install.ps1 | iex"
```

### **macOS / Linux**

터미널에서 아래 컬(curl) 명령어를 실행

```Bash
curl -sSf https://astral.sh/uv/install.sh | sh
```

## 2. 설치 확인하기

설치가 완료된 후 **터미널을 껐다가 다시 연 뒤**, 아래 명령어로 정상 설치를 확인

```
uv --version
```