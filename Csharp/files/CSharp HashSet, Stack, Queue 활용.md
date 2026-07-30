C#에서는 특정 목적에 맞춰 데이터를 효율적으로 다룰 수 있도록 **`HashSet<T>`**, **`Stack<T>`**, `Queue<T>`와 같은 특수 목적 제네릭 컬렉션(모두 **클래스/참조 타입**)을 제공합니다.

## 1. `HashSet<T>` (중복 없는 집합)

`HashSet<T>`은 **중복을 허용하지 않고, 요소의 순서가 없는** 자료구조입니다. 내부적으로 `Dictionary`와 유사한 해시 테이블(Hash Table)을 사용하여 데이터의 존재 여부를 탐색합니다.
### 🔑 주요 특징
- **중복 자동 제거:** 이미 존재하는 값을 추가하려고 하면 자동으로 무시됩니다.
- **압도적인 탐색 속도:** 특정 값이 집합에 있는지 확인(`Contains`)할 때 $O(1)$의 속도를 가집니다. (`List.Contains`는 $O(N)$)
- **순서 미보장:** 데이터가 들어간 순서가 유지되지 않습니다.
### 💻 활용 예시: 중복 제거 및 집합 연산
```cs
HashSet<string> visitedRooms = new HashSet<string>();

// 데이터 추가 (Add)
visitedRooms.Add("RoomA");
visitedRooms.Add("RoomB");
bool isAdded = visitedRooms.Add("RoomA"); // ❌ 중복! false 반환되며 추가되지 않음

// 포함 여부 검사 (Contains) - O(1)
if (visitedRooms.Contains("RoomA"))
{
    Console.WriteLine("이미 방문한 방입니다.");
}

// 집합 연산 (합집합, 교집합, 차집합 등)
HashSet<int> setA = new HashSet<int> { 1, 2, 3, 4 };
HashSet<int> setB = new HashSet<int> { 3, 4, 5, 6 };

setA.IntersectWith(setB); // setA는 교집합인 { 3, 4 }만 남음
```

## 2. `Stack<T>` (후입선출: LIFO)

`Stack<T>`은 **가장 나중에 들어간 데이터가 가장 먼저 나오는 (Last-In, First-Out)** 구조입니다. 내부적으로 가변 배열을 기반으로 동작합니다.
### 🔑 주요 연산
- **`Push(item)`**: 스택의 맨 위에 데이터 추가 ($O(1)$)
- **`Pop()`**: 스택의 맨 위 데이터를 **꺼내면서 제거** ($O(1)$)
- **`Peek()`**: 스택의 맨 위 데이터를 **제거하지 않고 조회만** 수행 ($O(1)$)
### 💻 활용 예시: 되돌리기(Undo), 웹 브라우저 뒤로가기

```cs
Stack<string> actionHistory = new Stack<string>();

// 작업 기록 저장
actionHistory.Push("이동");
actionHistory.Push("공격");
actionHistory.Push("아이템 사용");

// 현재 맨 위 데이터 확인 (Peek)
Console.WriteLine($"최근 행동: {actionHistory.Peek()}"); // "아이템 사용"

// Ctrl+Z (Undo): 마지막 작업 취소 (Pop)
string lastAction = actionHistory.Pop(); // "아이템 사용" 꺼냄
Console.WriteLine($"취소된 행동: {lastAction}");
```

## 3. `Queue<T>` (선입선출: FIFO)
`Queue<T>`는 **가장 먼저 들어간 데이터가 가장 먼저 나오는 (First-In, First-Out)** 구조입니다. 내부적으로 **원형 배열(Circular Array)** 구조로 구현되어 있어 맨 앞 데이터의 제거 연산이 매우 빠릅니다.
### 🔑 주요 연산
- **`Enqueue(item)`**: 큐의 맨 뒤에 데이터 추가 ($O(1)$)
- **`Dequeue()`**: 큐의 맨 앞 데이터를 **꺼내면서 제거** ($O(1)$)
- **`Peek()`**: 큐의 맨 앞 데이터를 **제거하지 않고 조회만** 수행 ($O(1)$)
### 💻 활용 예시: 대기열 시스템, 너비 우선 탐색(BFS)
```cs
Queue<string> matchingQueue = new Queue<string>();

// 매칭 대기열 등록 (Enqueue)
matchingQueue.Enqueue("Player1");
matchingQueue.Enqueue("Player2");
matchingQueue.Enqueue("Player3");

// 매칭 완료되어 대기열에서 처리 (Dequeue)
while (matchingQueue.Count > 0)
{
    string player = matchingQueue.Dequeue();
    Console.WriteLine($"{player} 님이 게임에 입장했습니다.");
}
```

## ⚖️ 핵심 컬렉션 3종 요약 비교

| **컬렉션**          | **주요 메커니즘**         | **데이터 탐색 (Contains)** | **적합한 사용 상황**                         |
| ---------------- | ------------------- | --------------------- | ------------------------------------- |
| **`HashSet<T>`** | 해시 테이블 (순서 X, 중복 X) | **$O(1)$**            | 고속 중복 검사, ID/방문 목록 관리, 집합 연산          |
| **`Stack<T>`**   | LIFO (후입선출)         | $O(N)$                | 되돌리기(Undo), UI 창 닫기 순서, DFS(깊이 우선 탐색) |
| **`Queue<T>`**   | FIFO (선입선출)         | $O(N)$                | 서버 요청 대기열, 매칭 시스템, BFS(너비 우선 탐색)      |