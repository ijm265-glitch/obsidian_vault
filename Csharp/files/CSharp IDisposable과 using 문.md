```cs
public interface IDisposable
{
	void Dispose();
}
```
> 이러한 형태의 IDisposable interface가 구현된 클래스에 대해 using을 사용할 수 있음
> C# 표준 클래스인 StreamWriter가 IDisposable이 구현된 대표적인 예시임

```cs
using (StreamWriter writer = new StreamWriter("test.txt"))
{
	writer.WriteLine("Hello C#!");
} // 중괄호를 나가는 순간 writer.Dispose()를 자동적으로 호출
```

```cs
public void WriteLog()
{
	using var writer = new StreamWriter("test.txt");
	writer.WriteLine("로직 실행중")
} // WriteLog() 메서드가 끝나는 시점에서 writer.Dispose()가 자동으로 실행
```