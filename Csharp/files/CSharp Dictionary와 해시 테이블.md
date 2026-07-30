| **구분**           | **Dictionary<TKey, TValue>** | **Hashtable**                   |
| ---------------- | ---------------------------- | ------------------------------- |
| **도입 시기**        | C# 2.0+ (권장)                 | C# 1.0 (구버전, 비권장)               |
| **타입 데이터**       | **제네릭 (Strongly Typed)**     | `object` 타입 (Non-Generic)       |
| **박싱/언박싱**       | **발생 안 함 (고성능)**             | 값 타입 저장 시 **박싱/언박싱 발생 (성능 저하)** |
| **타입 안전성**       | **컴파일 시점에 타입 검사**            | 실행 시점(Runtime)에 오류 감지 가능        |
| **존재하지 않는 키 접근** | `KeyNotFoundException` 예외 발생 | `null` 반환                       |
### 기본 사용법

```cs
// 제네릭으로 Key와 Value의 타입을 명확히 지정
Dictionary<string, int> scores = new Dictionary<string, int>();

// 데이터 추가
scores.Add("Alice", 95);
scores["Bob"] = 80; // 인덱서 구문으로 추가 또는 수정

// 키 존재 여부 확인 후 값 가져오기 (가장 안전한 방법: TryGetValue)
if (scores.TryGetValue("Alice", out int score))
{
    Console.WriteLine($"Alice의 점수: {score}"); // 🖥️ 출력: 95
}

// 안전하지 않은 접근 (키가 없으면 예외 발생!)
// int bobScore = scores["Charlie"]; // 💥 KeyNotFoundException!
```

### ② 성능 최적화: `Capacity` 미리 할당하기

`List<T>`와 마찬가지로 `Dictionary` 역시 내부 배열 크기가 모자라면 재할당(Resizing) 및 해시 재계산(Rehashing)이 일어납니다. 저장할 데이터 수를 대략 안다면 생성자에 크기를 지정해 주는 것이 성능상 유리합니다.

```cs
// 10,000개의 공간을 미리 할당하여 리사이징/재해싱 오버헤드 방지
Dictionary<int, string> userTable = new Dictionary<int, string>(1000);
```