# Server Programming
* * *
# 목차
 * [서버란](#서버란)
  * [멀티쓰레드](#멀티쓰레드)
  * [DeadLock](#DeadLock)
  * [SpinLock](#SpinLock)
  * [Sleep](#Sleep)
  * [Event](#Event)
  * [Condition Variable](#Condition-Variable)
  * [Future](#Future)
  * [Cache](#Cache)
  * [가시성/코드 재배치](#가시성과-코드-재배치)
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
```
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
* ##### volatile => 컴파일러 최적화 수행하지 마라
```
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
* CAS(Compare And Swap)   
-compare_exchange_strong
-compare_exchange_weak
=>두 함수가 모두 atomic 한 CAS 작업을 제공한다.
```
class CASExample 
{
private:
	bool locked = false;

public:
	bool lock() 
	{
		if(!locked) 
		{
    			locked = true;
    			return true;
  		}
 		 return false;
  	}
}
```
1. 위 클래스에서 lock 함수는 locked 변수의 값을 확인 후(체크) false 일경우 locked 변수를 true로 설정하고(액트) true리턴한다.   
2. 만약 두 쓰레드 A,B가 CASExample의 lock()함수에 동시에 접근한다면 A,B 쓰레드 모두 locked 변수를 false로 읽을 것이고,      
locked = true, return true 작업을 실행할 것이므로 A쓰레드가 lock을 할 경우 B쓰레드는 lock을 false리턴(실패)하는 의도와 다른 결과가 나타날 것이다.   
3. 따라서, 체크와 액트 실행 중에는 다른 쓰레드의 간섭을 받지 않고 원자적인 코드 블록을 실행 시켜야 한다.   
   (체크-액트가 하나의 원자적인 작업으로 오직 하나의 쓰레드에 의해 실행되어야함)      
compare_exchange_strong, compare_exchange_weak 함수가 원자적인 체크-액트 작업을 도와준다.

## Sleep
* SpinLock에서 반복문을 통해 락 상태를 확인하며 락을 계속 확인하는데 다른 쓰레드가 lock을 한시간이 길어지면 CPU 점유율이 높아질 수 있다.
* 이 때 Sleep을 통해 잠시 TimeSlice를 반환하여 CPU 부하를 낮출 수 있다.
* this_thread::sleep_for(쓰레드를 잠시 Block 상태로 한다. 일정 시간동안 스케줄러에 선택받지 못함),   
	this_thread::yield(쓰레드를 Ready 상태로 스케줄러에 의해 선택받을 수 있게 한다)
	
## Event
자동 Reset 이벤트
수동 Reset 이벤트
	
```
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
```
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
```
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
```
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
여러 쓰레드가 동일 메모리에 동시 접근 그 중 write가 문제가 된다
atomic is_lock_free 원자적으로 처리되었는가?

## Thread Local Stoage(TLS)
* 쓰레드들이 하나의 로직에 몰리지 않게 해야한다.   
* 쓰레드 마다 독립적으로 가지고 있는 공간   
* Heap이나 데이터영역(static 변수,전역변수)가 있는 공간에서 큰 덩어리를 떼어다가 TLS 영역에서 계산한다.   
* 매번 읽고/쓰기가 발생할 때마다 heap/데이터 영역에 위해 쓰레드가 몰리면 데이터 경합이 커지기 때문

