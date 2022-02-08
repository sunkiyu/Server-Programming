# Server Programming
* * *
# 목차
 * [서버란](#서버란)
  * [멀티쓰레드](#멀티쓰레드)
  * [DeadLock](https://github.com/sunkiyu/Server-Programming/blob/ea5804437b3fc1b0bc5c0e74fe50ab0eae16b99e/DeadLock/README.md)
  * [SpinLock](#SpinLock)
  * [CAS(Compare And Swap)](#CAS)
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
	
## DeadLock
* Lock 순서를 지켜준다.   
	=>A쓰레드에서 mutexA -> mutexB 순서로 Lock을 했을 경우 B쓰레드도 mutexA->mutexB 순서로 Lock을 한다.
* 개발단계에서는 일어나지 않다가 배포 후 다수의 사용자가 사용할 경우 간혹가다가 일어나는 경우가 대다수다. 
```cpp
mutex m1;
mutex m2;
std::lock(m1, m2); //교착상태 회피 알고리즘을 사용하여 lock 해준다.	
//adopt_lock 이미 lock 된 상태니까 나중에 풀어주기만 해라
//assume the calling thread already has ownership of the mutex
lock_guard<mutex> g1(m1, std::adopt_lock);
```
* lock_guard 객체는 Scope를 벗어날때 자동으로 unlock 시켜주기 때문에 데드락 현상 예방에 효과적이다.
* 알고리즘 그래프로 생각해볼 때 락의 사이클이 발생하면 데드락
* 유저레벨 커널레벨 컨텍스트 스위칭 부하 생각보다 크다. 	
### RAII(Resource Acquisition is Initialization)   
C++에서 객체가 생성되며 할당된 리소스가 Scope를 벗어나면 리소스를 해제해주는 설계 패턴.   
stack에 할당된 리소스는 Scope를 벗어나면서 소멸자가 호출되며 리소스가 해제되는 원리를 이용.   
new로 동적할당 받은 힙 메모리의 Resource는 프로그래머가 직접 관리해야 하지만 예외상황이난 exception 등의 이유로   
리소스 해제를 못하고 Memory Leak이 발생하거나 Mutex의 경우 lock/unlock을 제대로 수행하지 못해 교착상태에 빠질 수 있다.   
unique_ptr, shared_ptr, lock_guard 등으로 이러한 문제들을 예방할 수 있는데 Scope를 벗어나면 자동으로 리소스 해제를 시켜주기 때문이다.   
이렇게 함수 종료시 무조건 실행을 보장해주는 클래스들의 기능적인 부분들을 통틀어 RAII라고 부름

## SpinLock
* 스핀락이라는 이름은 락을 획득할 때까지 해당 스레드가 빙빙 돌고 있다(spinning)는 것을 의미
* 다른 쓰레드가 lock을 소유중이면 lock이 반환될 때까지 계속 확인하며 기다린다.
* 다른 쓰레드가 lock하는 시간이 짧을 때 유용하다. 루프를 돌면서 CPU를 양보하지 않으므로 불필요한 컨텍스트 스위칭을 막을 수 있다.
* 다른 쓰레드가 lock하는 시간이 길 경우 루프로 인해 CPU 점유율이 높아질 수 있다.(Busy Waiting)   
```cpp
class SpinLock
{
public:
	void lock()
	{
		bool expected=false, desired = true;

		while(_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}
	}

	void unlock()
	{
		_locked.store(false);
	}
private:
	atomic<bool> _locked = false;
};
```   
>SpinLock을 개발자가 Custom으로 구현시 BasicLockable 요구사항을 충족 시켜야한다. 
>BasicLockable을 lock 과 unlock을 구현한 클래스 등을 말한다. cppreference에 자세한 내용이 나와있다.   
>[https://en.cppreference.com/w/cpp/named_req/BasicLockable](https://en.cppreference.com/w/cpp/named_req/BasicLockable)   
* ##### volatile => 컴파일러 최적화 수행하지 마라
```cpp
int32 a=0;
a=1;
a=2;
a=3;
cout<<a<<endl;

//어셈블리 코드 => 컴파일러 최적화가 적용된 모습
00007FF6967710C0  mov         qword ptr [rsp+60h],rax  
int32 a = 0;
a = 1;
a = 2;
a = 3;
cout << a << endl;
00007FF6967710C5  mov         edx,3  

//어셈블리 코드 => volatile 키워드 적용
00007FF719E010C0  mov         qword ptr [rsp+60h],rax  
volatile int32 a = 0;
00007FF719E010C5  xor         ebx,ebx  
00007FF719E010C7  mov         dword ptr [rsp+30h],ebx  
	a = 1;
00007FF719E010CB  mov         dword ptr [a],1  
	a = 2;
00007FF719E010D3  mov         dword ptr [a],2  
	a = 3;
00007FF719E010DB  mov         dword ptr [a],3  
cout << a << endl;
```   
Release 모드 최적화 적용된다.
## CAS  
-compare_exchange_strong
-compare_exchange_weak
=>두 함수가 모두 atomic 한 CAS 작업을 제공한다.
```cpp
#include<iostream>
#include<thread>

int sum = 0;

void Add()
{
    for (int i = 0; i < 100000000; i++)
    {
        sum++;
    }
}

int main(int argc, char *argv[])
{
    std::thread t1(Add);
    std::thread t2(Add);

    t1.join();
    t2.join();

    std::cout << sum << std::endl;
    return 0;
}
```   
위의 코드를 실행하면 원칙적으로는 200000000이 출력되어야한다.   
sum이 thread1과 thread2를 통해 각각 100000000 X 1번씩 증가하였기 때문이다.   
하지만 결과는?   
![image](https://user-images.githubusercontent.com/68372094/151298276-2b581cad-fcb9-4cbc-bee3-60bc4fcc5ab8.png)   
위와 같이 200000000과는 전혀 다른 엉뚱한 값이 나오게 된다.   
sum이 공유 변수이기 때문에 멀티스레드 환경에서 DataRace 환경에 처했기 때문이다.   
sum이 1 증가할 때 생각보다 많은 작업이 일어나는데   
1. 메모리에서 sum의 값을 레지스터로 fetch 한다.   
2. CPU ALU에서 증가 연산   
3. 메모리에 증가된 값을 저장   
1부터 3까지의 과정이 원자적으로 *한번에 하나의 스레드에 의해 처리되어야* 하지만 멀티스레드 환경에서는 1~3 과정 중 다른 스레드가 침범하여 공유 변수값을 오염시킬 우려가 있다.   
흔히 함수 호출 단위로 CPU가 명령어를 처리하는 것으로 생각하지만 단순한 cout 호출시에도 Context Switching 과정이 일어나기 때문에 의도와 다른 값이 나올 수 있다.
따라서 1부터 3까지의 과정을 원자적으로 처리해야 한다.   
```cpp   
#include<iostream>
#include<queue>
#include<mutex>
#include<thread>
#include<atomic>

int sum = 0;
std::mutex m;
std::atomic<bool> _locked= {false};

void Add()
{
    for (int i = 0; i < 100000000; i++)
    {
        bool expected=false;
        bool desired = true;

        while(_locked.compare_exchange_strong(expected, desired) == false)
        {
            expected = false;
        }
        sum++;
         _locked.store(false);
    }
}
				  
int main(int argc, char *argv[])
{
    std::thread t1(Add);
    std::thread t2(Add);

    t1.join();
    t2.join();

    std::cout << sum << std::endl;
    return 0;
}
```   
![image](https://user-images.githubusercontent.com/68372094/151299019-cd5a4ffd-0398-4ff4-8e65-b43c8599d44a.png)   
위와 같이 CAS (Compare And Swap)을 적용하면 1~3의 과정을 원자적으로 처리하여 우리가 의도한 값 200000000을 얻을 수 있다.   
원자적 처리 과정은 아래와 같다.   
1. 스레드1이 먼저 Add 함수의 루프에서 compare_exchange_strong 함수를 호출한다.
2. 스레드1이 최초로 compare_exchange_strong을 호출하였기 때문에 _locked와 expected는 모두 false를 나타낸다.   
3. 따라서 _locked는 desired 값으로 변경된 후(_locked == desired == true) true를 리턴하여 while문을 빠져 나온뒤 sum을 증가시키는 연산을 한다.
4. 이때 스레드2도 실행 중이므로 sum 공유 변수를 건드리려 하는데 스레드 2가 compare_exchange_strong를 호출한 시점은   
   3번 과정에서 _locked== true, expected==false 이므로 expected = _locked 값으로 수정한 뒤 false를 리턴하고 _locked(true)로 수정된 expected를 false로 수정한다.   
5. false를 리턴하였으므로 다시 while 문에서 compare_exchange_strong을 호출하는데 4번과 마찬가지 상태이므로 while 루프에 머물면서 sum 공유 변수를 건드리지 못한다.
6. sum 증가 연산을 한뒤 비로소 _locked.store ==false를 호출하는데 이때 while문에 머물던 스레드2 가 _locked== false, expected == false 상태로 true를 리턴하고 sum   
   공유변수의 값을 증가시킬 기회를 얻는다.
7. 위와 같은 과정이 두 스레드 모두 루프를 돌 때까지 반복된다.   
				 
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
