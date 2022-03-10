## ThreadManager   
스레드를 생성 및 파기 관리한다.
```cpp
class ThreadManager 
{
public:
	ThreadManager();
	~ThreadManager();

	void Launch(function<void(void)> callback);
	void Join();

	static void InitTLS();
	static void DestroyTLS();
private:
	Mutex			_lock;
	vector<thread> _threads;
};
```
