# Server Programming
* * *
# 목차
 * [서버란](#서버란)
  * [멀티쓰레드](#멀티쓰레드)
  * [DeadLock](https://github.com/sunkiyu/Server-Programming/blob/ea5804437b3fc1b0bc5c0e74fe50ab0eae16b99e/DeadLock/README.md)
  * [SpinLock](https://github.com/sunkiyu/Server-Programming/blob/514193b66bb37ae9b7a6032012482cda0f6a470c/SpinLock/README.md)
  * [CAS(Compare And Swap)](https://github.com/sunkiyu/Server-Programming/blob/083f88889266a8e1e7d28d7cb19140e4b39d4522/CAS/README.md)
  * [Sleep](#Sleep)
  * [Event](#Event)
  * [Condition Variable](#Condition-Variable)
  * [Future](#Future)
  * [Cache](#Cache)
  * [가시성/코드 재배치](#가시성과-코드-재배치)
  * [메모리 모델](#메모리-모델)
  * [Thread Local Stoage(TLS)](#Thread-Local-Stoage)
  * [Lock-Based Stack](#Lock-Based-Stack)
  * [Lock-Based Queue](#Lock-Based-Queue)
  * [Lock-Free-Stack#1](#Lock-Free-Stack1)
  * [Lock-Free-Stack#2](#Lock-Free-Stack2)
  * [ThreadManager](#ThreadManager)
  * [Reader-Writer-Lock](#Reader-Writer-Lock)
  * [Dead-Lock 탐지](#Dead-Lock-Detection)
  * [멀티스레드 소수구하기 문제](#멀티스레드-소수-구하기)
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
			 
## Sleep
* SpinLock에서 반복문을 통해 락 상태를 확인하며 락을 계속 확인하는데 다른 쓰레드가 lock을 한시간이 길어지면 CPU 점유율이 높아질 수 있다.
* 이 때 Sleep을 통해 잠시 TimeSlice를 반환하여 CPU 부하를 낮출 수 있다.
* this_thread::sleep_for(쓰레드를 잠시 Block 상태로 한다. 일정 시간동안 스케줄러에 선택받지 못함),   
	this_thread::yield(쓰레드를 Ready 상태로 스케줄러에 의해 선택받을 수 있게 한다)
	
## Event
자동 Reset 이벤트
수동 Reset 이벤트
	
```cpp
void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}
		::SetEvent(handle);
		this_thread::sleep_for(100000000ms);
	}
};

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		::WaitForSingleObject(handle, INFINITE);
		//::ResetEvent(handle);
		//Signal 되었다가 Non-Signal 상태로 변함
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
};
```
* CreateEvent,CreateProcess 등을 수행하면 커널 오브젝트가 생성된다.    
* UsageCount는 생성된 커널 오브젝트를 참조하는 횟수인데 CreateProcess일 경우 부모 프로세스, 자식 프로세스 총 2개의 UsageCount가 발생한다.   
* CloseHandle을 호출하게 되면 UsageCount가 감소하게 되는데 UsageCount가 0이 되었을때 해당 커널 오브젝트가 소멸된다.   
* Handle을 커널 오브젝트 정보를 참조하는 역할을 한다.
## Condition Variable   
* Event는 커널 영역에서 실행 Condition Variable은 유저 영역에서 실행된다.
* condition_variable 클래스는 멀티쓰레드 환경에서 대기와 통지를 통해 공유 변수를 안전하게 수정할 수 있다.   
* 보통 뮤텍스 함께 사용하여 Thread Safe하다.
* condition_variable 클래스는 다른 스레드가 공유 변수(조건)를 수정하고, condition_variable을 알릴 때까지    
  하나의 스레드 또는 동시에 여러 스레드를 차단하는 데 사용할 수 있는 동기화 기본 요소.
-공유 변수를 수정하려는 스레드는 다음을 수행해야 한다.
	1. std::mutex를 획득해야 한다. (일반적으로 lock_guard를 통해 획득한다.)
	2. 락이 걸린 상태에서 공유 변수를 수정한다.
	3. std::condition_variable에 대해 notify_one 또는 notify_all을 통해 깨운다.(락을 푼다.).

* 공유 변수가 atomic 한 경우에도 대기 스레드에 수정 사항을 올바르게 전달하려면 뮤텍스 락을 건 후 수정해야 함.
- std::condition_variable에서 대기하는 모든 스레드는 다음을 수행.
	1. 공유 변수를 보호하는 데 사용된 동일한 뮤텍스에서 std::unique_lock<std::mutex> 획득
	2. wait, wait_for 또는 wait_until을 실행. 대기 작업은 atomic하게 뮤텍스를 해제하고 스레드 실행을 일시 중단.
	3. 조건 변수가 알림을 받거나 시간 초과가 만료되거나 조건 충족이 실패하면 스레드가 깨어나고 뮤텍스가 원자적으로 다시 획득됩니다. 
	  그런 다음 스레드는 조건을 확인하고 조건 충족이 실패한 경우 대기를 재개.
## Future
* 비동기적 실행
> 동기 - 함수 호출 후 I/O 작업 및 로직 처리를 마친 시점과 함수의 리턴 시점이 같다.   
비동기 - 함수 호출 후 바로 리턴한다(관심있는 이벤트를 운영체제에 맡기거나 등록해놓는다),   백그라운드에서 I/O작업 및 로직처리를 한 뒤 완료되면 프로그램에 알린다.
* deferred ->lazy evaluation 지연해서 실행
* async ->별도의 쓰레드를 만들어서 실행
```cpp
std::future<int64> future = std::async(std::launch::async, Process);
	
future.wait(); // future.wait_for(INFINITE);
std::future_status status = future.wait_for(1ms);
if (status == future_status::ready)
{

}
int64 sum = future.get(); 
```

## Cache
* RAM 보다 CPU에 가까우며 속도가 빠르다. 처리 속도(레지스트리>캐시>램>하드디스크)
* RAM이나 하드디스크에서 CPU에서 매번 처리할 데이터를 꺼내오는 것은 지연시간이 오래 걸린다.
* CPU가 RAM에서 데이터를 꺼내기 전 캐시를 확인한다.
* Tmeporal Locality =>한번 사용된 데이터는 재사용될 확률이 높다.
* Spatial Locality => 사용된 데이터의 주변 데이터는 사용될 확률이 높다.
	
* Spatial Locality의 예(Cache Friendly Code)
```cpp
INT32 buffer[10000][10000];
int main(int argc, char *argv[])
{

    memset(buffer, 0, sizeof(buffer));
	//열 단위로 접근
	{
		UINT64 start = GetTickCount();
		INT64 sum = 0;
		for(int i=0; i<10000; i++)
			for (int j = 0; j < 10000; j++)
			{
				sum += buffer[i][j];
			}
		UINT64 end = GetTickCount();
		cout << end - start << endl;
	}

	//행 단위로 접근
	{
		UINT64 start = GetTickCount();
		INT64 sum = 0;
		for (int i = 0; i<10000; i++)
			for (int j = 0; j < 10000; j++)
			{
				sum += buffer[j][i];
			}
		UINT64 end = GetTickCount();
		cout << end - start << endl;
	}

}
```
![image](https://user-images.githubusercontent.com/68372094/150241439-105f7d46-38d8-4fb2-9c4f-788bfc46fcc4.png)

## 가시성과 코드 재배치
* CPU 처리 순서 
> 1. Fetch 
> 2. Decode 
> 3. Excecute 
> 4. Write-Back

* 컴파일러와 CPU가 최적화를 위해 가시성과 코드 재배치를 수행하여 의도하는 값과 다른 값이 나올 수 있다.
* 가시성 - 멀티 코어 환경에서는 캐시와 레지스트리가 여러개 존재하여 멀티쓰레드가 공유 변수에 접근할 때 서로 다른 캐시와 레지스터에 읽고/쓰기 할 수 있는데
	  이 때 시간차이와 DataRace등 경합 상태가 발생하여 쓰레드 별로 서로 다른 값을 바라 볼 수 있다.
* 코드 재배치 - 컴파일러와 CPU가 효율적인 작업이라고 판단하여 최적화 시켜 코드 재배치가 일어날 수 있는데 이로 인해 의도하는 값과 다른 값이 나올 수 있다.
* 코드 재배치의 예
```cpp
int32 x = 0,y = 0,r1 = 0,r2 = 0;

volatile bool ready;

void Thread_1()
{
	while (!ready);
	y = 1; //store y
	r1 = x; //x load
}
	
void Thread_2()
{
	while (!ready);
	x = 1; //store y
	r2 = y; //x load
}

int main()
{
	int32 count = 0;

	while (true)
	{
		ready = false;
		count++;
		x = y = r1 = r2 = 0;

		thread t1(Thread_1);
		thread t2(Thread_2);
		
		ready = true;

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;
	}
	//r1 == 0 && r2 == 0인 상황이 나올 수 없는 구조지만 코드 재배치를 통해 
	//r1 == 0 && r2 == 0인 상황이 발생함
	cout << count << "-r1==0 && r2==0" << endl;
}
```
## 메모리 모델
* 여러 쓰레드가 동일 메모리에 동시 접근 그 중 write가 문제가 된다
* atomic is_lock_free 원자적으로 처리되었는가?
```cpp
atomic<bool> flag;
//true =>원래 원자적으로 처리되었다.(된다) 
//false 원자적 처리가 필요하다.
flag.is_lock_free();

//flag 값을 prev에 넣기 전에 다른 쓰레드가 수정할 수 있다.
//이전 값을 넣고 true로 바꾸겠다. 아토믹한 연산
bool prev = flag.exchange(true);
	
//CAS(compare and swap) 조건부 수정
{
	//expected 예상한값. flag와 expected 가 같은 값인가? 그럼 desired로 바꾼다.
	bool expected = false;
	bool desired = true;
	flag.compare_exchange_strong(expected, desired);
}
```
### Memory model 정책

#### 1.Sequentially Consistent(seq_cst)
#### 2.Acquire-release(consume,acquire,release,acq_rel)
#### 3.Relaxed(relaxed)

* __seq_cst__(가장 엄격)   
-컴파일러 최적화 여지 적음 직관적, atomic 동작원리   
-가시성 코드재배치 바로 해결
	
* __acquire_release__ 핵심   
-seq_cst보다 덜 엄격하다   
-release 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지   
-acquire 로 같은 변수를 읽는 스레드가 있다면 release 이전의 명령들이 acquire하는 순간에 관찰 가능 가시성 보장   

* __consume__    
-사용 빈도 낮다.   

* __relaxed__   
-자유롭다      
-컴파일러 최적화 여지 많음 직관적이지 않음      
-코드재배치 가시성 해결 못한다.   
-가장 기본 조건 동일 객체에 대한 동일 관전 순서만 보장   

* Intel, AMD의 경우 순차적 일관성을 보장하므로 seq_cst를 써도 별다른 부하 없음   
* ARM의 경우 차이가 있다.   
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

## Lock-Based Stack   
### * Lock을 걸지 않은 Stack과 Queue는 멀티스레드 환경에서 비어있는 값을 참조하는 Crash가 발생할 가능성이 있다.
### Push & Pop의 예   
				 
```cpp
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
```   
## Lock-Based Queue   
### * Lock을 걸지 않은 Stack과 Queue는 멀티스레드 환경에서 비어있는 값을 참조하는 Crash가 발생할 가능성이 있다.
### Push & Pop의 예   
				 
```cpp
	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T &value)
	{
		//Lock 을 걸고
		lock_guard<mutex> lock(_mutex);
		if (_queue.empty())
			return false;

		//
		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		//조건 만족하지  않으면 Lock 을 풀고 잠든다.
		//시그널이 오면 Lock을 잡고 실행
		_condVar.wait(lock, [this] {return _queue.empty() == false; });
		value = std::move(_queue.front());
		_queue.pop();
	}
```
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
## 멀티스레드 소수 구하기   
```cpp
#include <atomic>
#include <windows.h>
#include <iostream>
#include <thread>
using namespace std;
atomic_int ssAtomic;

void ssCount(int start,int end)
{
    for(int num =start; num<=end; num++)
    {
        if(num==1){
            continue;
        }
        bool isss= true;
        for(int i=2; i<num; i++)
        {
            if(!(num%i)){
                isss = false;
                break;
            };
        }
        if(!isss)continue;
        ssAtomic++;
    }
}

int main(int argc, char *argv[])
{
   int start = GetTickCount();
   thread t1(ssCount,1,300000);
   t1.join();
   int end = GetTickCount();
   
   cout<<"Total= "<<ssAtomic<<" "<<end-start<<"ms"<<endl;
   return 0;
}
```   
![image](https://user-images.githubusercontent.com/68372094/152285760-ffc3039e-8525-4565-bc6b-a03eaf32f0fb.png)   
싱글스레드 환경에서는 300,000까지 소수의 개수를 구하는데 15875ms(약15초)의 시간이 소요된다.      
```cpp
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
atomic_int ssAtomic;

void ssCount(int start,int end)
{
    for(int num =start; num<=end; num++)
    {
        if(num==1){
            continue;
        }
        bool isss= true;
        for(int i=2; i<num; i++)
        {
            if(!(num%i)){
                isss = false;
                break;
            };
        }
        if(!isss)continue;
        ssAtomic++;
    }
}

int main(int argc, char *argv[])
{
   int start = GetTickCount();
   vector<thread> threads;
   int coreCnt = thread::hardware_concurrency();
   
	threads.push_back(thread(ssCount,1,70000));
	threads.push_back(thread(ssCount,70001,140000));
	threads.push_back(thread(ssCount,140001,210000));
	threads.push_back(thread(ssCount,210001,300000));
	 
   for(int i=0; i<4; i++)
   {
   	threads[i].join();
   }
   int end = GetTickCount();
   
   cout<<"Total= "<<ssAtomic<<" "<<end-start<<"ms"<<endl;
   return 0;
}
```
![image](https://user-images.githubusercontent.com/68372094/152287019-2b2ddfc7-e8c8-4377-9fde-fee25cf7d056.png)   
hardware_concurrency를 통해 PC코어 개수를 확인해보니 4개였다. 따라서 스레드를 4개 이상 생성하는 것은 이 프로젝트에서 큰 의미가 없어보이므로 스레드 4개를 생성후 실험하였다.   
9087ms(약 9초)로 6초 가량 시간이 단축되는 것을 확인 할 수 있었다.   
![image](https://user-images.githubusercontent.com/68372094/152288482-1b3e1993-226c-4efb-8627-e4817dca3f05.png)   
4코어를 모두 높은 사용률을 나타낸다. 4개 코어가 4개의 스레드를 담당하기 때문   
![image](https://user-images.githubusercontent.com/68372094/152288999-4d71e802-b26b-48e7-9cab-53513d76a97a.png)   
싱글스레드일 경우 컨텍스트 스위칭을 통해 CPU부하를 나누기 때문에 부하가 덜하다. 4개의 코어가 1개의 스레드를 담당   
## 레퍼런스-카운팅   
* unique_ptr은 C스타일 포인터와 거의 같다. 차이점은 복사하는 부분(복사 생성자)이 막혀있다. std::move를 통해서(이동 대입 연산자) 대입 연산 가능   
* weak_ptr은 shared_ptr의 순환 참조 문제 해결.   
* shared_ptr 사용시 reference Count도 같이 관리된다.   
* weak_ptr은 reference Count만 체크한다. 객체의 수명 주기에는 영향을 미치지 않는다.   
## Allocator   
## Stomp-Allocator
