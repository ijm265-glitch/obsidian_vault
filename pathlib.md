경로를 문자열이 아닌 **객체(Object)** 형태로 직관적이고 안전하게 다룸
```python
from pathlib import Path
p = Path("C/Users/test/data/image.jpg")
```
## 1. 경로 정보 추출 (Properties / Attributes)

|**속성명**|**설명**|**출력 예시 (image.jpg 기준)**|
|---|---|---|
|**`p.name`**|확장자를 포함한 전체 파일명 (문자열)|`'image.jpg'`|
|**`p.stem`**|확장자를 제외한 파일 이름 (문자열)|`'image'`|
|**`p.suffix`**|파일 확장자 (문자열)|`'.jpg'`|
|**`p.suffixes`**|이중 확장자 리스트 (`.tar.gz` 다룰 때 유용)|`['.tar', '.gz']`|
|**`p.parent`**|바로 위 상위 디렉토리 경로 (Path 객체)|`Path('C:/Users/test/data')`|
|**`p.parents`**|상위 모든 부모 디렉토리 조상 요소들|`[Path('.../data'), Path('.../test'), ...]`|
|**`p.parts`**|경로를 구성하는 개별 조각 튜플|`('C:\\', 'Users', 'test', 'data', 'image.jpg')`|

## ## 2. 경로 조작 및 결합 (Path Manipulation)

- **`p1 / p2 / "file.txt"` 연산자**
    - 슬래시 연산자로 경로를 직관적으로 결합합니다. (OS 상관없이 자동으로 슬래시/백슬래시 처리)
    ```Python
    base = Path("./yolo_dataset")
    target = base / "images" / "train"  # ./yolo_dataset/images/train
    ```
- **`p.resolve()`**
    - 상대 경로(예: `./data`)를 시스템의 **절대 경로**(예: `C:/Users/.../data`)로 완전 변환합니다.
        
- **`p.with_name("new.png")`**
    - 경로에서 파일 이름만 변경된 새로운 Path 객체를 반환합니다.
        
- **`p.with_suffix(".txt")`**
    - 확장자만 바꾼 새로운 Path 객체를 반환합니다. (JSON $\rightarrow$ TXT 라벨 변환할 때 필수!)
    ```Python
    json_path = Path("label.json")
    txt_path = json_path.with_suffix(".txt")  # label.txt
    ```
## 3. 디렉토리 & 파일 관리 (File & Directory Operations)

- **`p.mkdir(parents=True, exist_ok=True)`**
    - 디렉토리를 생성합니다.
    - `parents=True`: 상위 폴더까지 한꺼번에 생성 (`os.makedirs` 역할)
    - `exist_ok=True`: 이미 폴더가 존재해도 에러를 발생시키지 않음
        
- **`p.exists()`**
    - 해당 경로에 파일이나 폴더가 실제로 존재하는지 확인 (`True`/`False`)
        
- **`p.is_file()` / `p.is_dir()`**
    - 해당 경로가 파일인지 / 디렉토리인지 판별
        
- **`p.unlink(missing_ok=True)`**
    - **파일을 삭제**합니다. (`missing_ok=True`: 파일이 없어도 에러 안 남)
        
- **`p.rmdir()`**
    - **비어있는 디렉토리를 삭제**합니다.
        

## 4. 탐색 (File Searching)

- **`p.glob(pattern)`**
    - 현재 폴더에서 패턴에 일치하는 파일들을 찾는 제너레이터(`Iterator[Path]`)를 반환합니다.
    ```Python
    images = p.glob("*.jpg")
    ```
    
- **`p.rglob(pattern)`**
    - 모든 하위 디렉토리까지 재귀적으로 탐색(Recursive Glob)하는 제너레이터를 반환합니다.
    
    ```Python
    # p.glob("**/*.json") 과 완전히 동일합니다.
    all_json = p.rglob("*.json") 
    ```
## 5. 파일 직접 읽기 & 쓰기 (Simple I/O)

- **`p.read_text(encoding='utf-8')`**: 파일 전체 텍스트를 문자열로 읽어옴
- **`p.write_text("내용", encoding='utf-8')`**: 텍스트를 파일에 덮어씀
- **`p.read_bytes()` / `p.write_bytes()`**: 바이너리 데이터 읽기/쓰기