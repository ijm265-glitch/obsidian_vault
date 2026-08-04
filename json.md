[[Context Manager]]
JSON 형태의 텍스트 데이터를 파이썬 Dictionary/List 객체로 변환

```python
with open('data.json', 'r') as f:
	data = f.read()
```

1. `with open('data.json', 'r') as f`: 파일 스트림을 생성하고 OS로부터 파일 핸들을 받아옴
2. `data = json.load(f)`: 파일 핸들을 이용하여 데이터를 json형식으로 load함