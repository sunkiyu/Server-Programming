# Server Programming
* * *
# 목차
 * [서버란](#서버란)
  * [멀티쓰레드](#멀티쓰레드)
  * [DeadLock](https://github.com/sunkiyu/Server-Programming/blob/ea5804437b3fc1b0bc5c0e74fe50ab0eae16b99e/DeadLock/README.md)
  * [SpinLock](https://github.com/sunkiyu/Server-Programming/blob/514193b66bb37ae9b7a6032012482cda0f6a470c/SpinLock/README.md)
  * [CAS(Compare And Swap)](https://github.com/sunkiyu/Server-Programming/blob/083f88889266a8e1e7d28d7cb19140e4b39d4522/CAS/README.md)
  * [Sleep](https://github.com/sunkiyu/Server-Programming/blob/35042c521d923b0afc54bfb8b4fef260164ccf2e/Sleep/README.md)
  * [Event](https://github.com/sunkiyu/Server-Programming/blob/865fdadd86661e9e183d2dd3c91d7b3d6b65f9d6/Event/README.md)
  * [Condition Variable](https://github.com/sunkiyu/Server-Programming/blob/e03faf4da32ef2e15b9d35849c2b5c1c5484db15/Condition%20Variable/README.md)
  * [Future](https://github.com/sunkiyu/Server-Programming/blob/bb8922b00295ba37977bdfd6eefc5149f21121d7/Future/README.md)
  * [Cache](https://github.com/sunkiyu/Server-Programming/blob/25ee0f9553e0666642a3705734ce01137a60ccac/Cache/README.md)
  * [가시성/코드 재배치](https://github.com/sunkiyu/Server-Programming/blob/1d09a1dc94c99a6e2145a2052d3aebb79dbe67c6/%EA%B0%80%EC%8B%9C%EC%84%B1-%EC%BD%94%EB%93%9C%20%EC%9E%AC%EB%B0%B0%EC%B9%98/README.md)
  * [메모리 모델](https://github.com/sunkiyu/Server-Programming/blob/1daf24d651a6544c348e586dde454bfeedcd97ef/Memory%20Model/README.md)
  * [Thread Local Stoage(TLS)](https://github.com/sunkiyu/Server-Programming/blob/bdb74293bc6638e5ffad988c3ed7065658f5d692/TLS/README.md)
  * [Lock-Based Stack](https://github.com/sunkiyu/Server-Programming/blob/39b0bcae6e48a495601073a1005a9830f67982fa/LockBased%20Stack%20Queue/README.md)
  * [Lock-Based Queue](https://github.com/sunkiyu/Server-Programming/blob/39b0bcae6e48a495601073a1005a9830f67982fa/LockBased%20Stack%20Queue/README.md)
  * [Lock-Free-Stack#1](#Lock-Free-Stack1)
  * [Lock-Free-Stack#2](#Lock-Free-Stack2)
  * [ThreadManager](#ThreadManager)
  * [Reader-Writer-Lock](#Reader-Writer-Lock)
  * [Dead-Lock 탐지](#Dead-Lock-Detection)
  * [멀티스레드 소수구하기 문제](https://github.com/sunkiyu/Server-Programming/blob/273667fa034ad9ce06a50c8f0608ff9f5abf1922/%EB%A9%80%ED%8B%B0%EC%8A%A4%EB%A0%88%EB%93%9C%20%EC%86%8C%EC%88%98%20%EA%B5%AC%ED%95%98%EA%B8%B0/README.md)
  * [레퍼런스 카운팅](#레퍼런스-카운팅)
  * [Allocator](#Allocator)
  * [Stomp Allocator](#Stomp-Allocator)
* * *
## 서버란
-다른 컴퓨터에서 연결 가능하도록 상시 실행대기하며 서비스를 제공하는 프로그램

### Web Server(예 HTTP Server)
* 정보 요청/갱신이 적다
* 실시간 인터랙션이 없다.
* 웹 서비스를 만드는데 주로 사용(구글,네이버,아마존 등)
* 처음부터 만들지 않고 프레임웍을 하나 골라서 사용
  (Spring(Java), Node JS(JavaScript),vASP.net(C#),vPHP)

### Game Server(예 TCP Server, Binary Server)
* 요청/갱신 많다.
* 실시간 인터랙션 있다.
* 서버가 클라이언트에 질의 가능
* stateful(상태 갱신 및 교환이 많다)

### 게임서버 작성시 고려대상
* 최대 동시 접속자
* 게임 장르 및 채널링
* 게임 로직
* 쓰레드 개수
* 쓰레드 모델
* 네트워크 모델
* 반응성
* DB

## 멀티쓰레드 
* <windows.h>에 있는 CreateThread는 윈도우에 종속적. 
* C++ 11에서 지원하는 thread를 include하여 std::thread를 사용하는 것이 범용성에 좋다.
* join() 함수로 쓰레드가 끝날때까지 기다린 후 종료한다. vector<>에 thread를 여러개 넣어놓고 관리할 수 있다.
* 전역변수 동기화 문제
> 전역변수 값 변경시 레지스트리와 메모리 사이에 읽기 쓰기 하는 과정 중    
  다른 쓰레드도 동시에 작업을 하기 때문에 덮어쓰기 문제가 발생하여 원하는 값이 나오지 않을 수 있다.   
  interlockedincrement 함수를 사용하거나 atomic을 include 하여 atomic<int32>와 같이 사용하면 해당 문제를 해결할 수 있다.       
  ->속도 저하 문제가 생길 수 있으므로 꼭 필요한 경우에만 사용하자   
## Lock-Free Stack1
```cpp
template<typename T>
class LockStack
{
public:
	...
	
	void Push(T value)
	{
		//Lock 을 걸고
		lock_guard<mutex> lock(_mutex);
		//우측값으로 변환후 넣는다.
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T &value)
	{
		//Lock 을 걸고
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		//
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		//조건 만족하지  않으면 Lock 을 풀고 잠든다.
		//시그널이 오면 Lock을 잡고 실행
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}
		 
private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};
```
## Lock-Free Stack2   
Node 데이터 구조를 이용한다.   
```cpp
struct Node
{
	Node(const T& value) :data(value),next(nullptr)
	{

	}

	T data;
	Node *next;
};
```

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
## Reader-Writer-Lock   
* 스레드와 락 관리
```cpp
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)

4바이트(32비트) 값으로 ThreadId와 락을 공유하는 스레드 개수 정보를 나타낸다.
상위 2바이트(16비트) => ThreadId
하위 2바이트(16비트) => 락을 공유하는 스레드 개수

 enum : uint32
    {
    //Dead락을 감지하는 TimeOut 시간
        ACQUIRE_TIMEOUT_TICK = 10000,
    //락을 시도하는 횟수
        MAX_SPIN_COUNT = 5000,
    //상위 16비트를 뽑아낸다. =>스레드 아이디
        WRITE_THREAD_MASK = 0xFFFF'0000,
    //하위 16비트를 뽑아낸다. => 공유 락 횟수
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };
```cpp
//쓰레드 ID 관리
thread_local uint32 LThreadId = 0;
 Atomic<uint32> _lockFlag = EMPTY_FLAG;

//쓰기 락
void Lock::WriteLock()
{
	//시작 시간을 얻는다.
	const int64 beginTick = ::GetTickCount64();
    //스레드 아이디를 얻는다.(2바이트)
    //32비트를 왼쪽으로 16비트 쉬프트하게 되면 16비트가 남는데
    //그중 상위 16비트를 뽑아내어 (스레드 아이디)
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
            //_lockFlag가 expected와 같으면(EMPTY_FLAG)
            //_lockFlag = desired해주고 return true;
            //같지 않으면 expected = lockFlag해주고 return false;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}

//시간 차이를 구해 Tick보다 크면 데드락으로 간주해 Crash를 낸다.
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");
//자신의 실행 기회를 잠시 다른 스레드에게 양보한다.
		this_thread::yield();
	}
}

void Lock::ReadLock()
{
    //스레드 아이디를 뽑아낸다.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPL
    
```   

## Dead-Lock Detection   
* 데드락 상황은 nullptr 체크 누락이 거의 0 순위로 제일 많은 원인이 된다.   
* 데드락은 그래프 알고리즘의 사이클 발생 여부로 판별할 수 있다.   
- 순방향 간선   
- 교차 간선   
- 역방향 간선   

## 레퍼런스-카운팅   
* unique_ptr은 C스타일 포인터와 거의 같다. 차이점은 복사하는 부분(복사 생성자)이 막혀있다. std::move를 통해서(이동 대입 연산자) 대입 연산 가능   
* weak_ptr은 shared_ptr의 순환 참조 문제 해결.   
* shared_ptr 사용시 reference Count도 같이 관리된다.   
* weak_ptr은 reference Count만 체크한다. 객체의 수명 주기에는 영향을 미치지 않는다.   
## Allocator   
## Stomp-Allocator
