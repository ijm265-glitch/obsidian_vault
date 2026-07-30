```python
sys.args # 터미널에서 코드 파일을 실행할 때의 인자를 가져옴

```

## sys.args
```python
import sys
print(sys.args)
# terminal에서 python main.py로 해당 코드 파일을 실행했을 때
# print(sys.args) -> ['main.py']
# python main.py video.mp4로 해당 코드 파일을 실행했을 때 
# # print(sys.args) -> ['main.py', 'video.mp4']
```