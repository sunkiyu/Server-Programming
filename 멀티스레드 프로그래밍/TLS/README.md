## Thread Local Stoage
### TLS
* 쓰레드들이 하나의 로직에 몰리지 않게 해야한다.   
* 쓰레드 마다 독립적으로 가지고 있는 공간(스레드별로 독립적으로 가지고 있는 스택영역과 같다)   
* Heap이나 데이터영역(static 변수,전역변수)가 있는 공간에서 큰 덩어리를 떼어다가 TLS 영역에서 계산한다.   
* 매번 읽고/쓰기가 발생할 때마다 heap/데이터 영역에 위해 쓰레드가 몰리면 데이터 경합이 커지기 때문
```cpp
//windows 용 TLS
_declspec(thread) int32 value;
//c++ 11
//쓰레드마다 자기만의 공간을 갖는다.
//thread_local 이 없을 경우 LThreadId가 덮어써진다.
thread_local int32 LThreadId = 0;

void ThreadMain(int32 threadId)
{
	LThreadId = threadId;

	while (true)
	{
		cout << "Hi i am Thread" << LThreadId << endl;
		this_thread::sleep_for(1s);
	}
}

int main()
{
	vector<thread> threads;
	for (int32 i = 0; i < 10; i++)
	{
		int32 threadId = i + 1;
		threads.push_back(thread(ThreadMain,threadId));
	}

	for (thread &t : threads)
	{
		t.join();
	}
}
```
* thread local storage 사용 안했을 경우   
![image](https://user-images.githubusercontent.com/68372094/150449289-d3568e94-52dc-40e9-84dd-b87e0fc66b38.png)
* thread local storage 사용 했을 경우   
![image](https://user-images.githubusercontent.com/68372094/150449405-146901a8-654d-4842-b64d-a88a47a59d42.png)
