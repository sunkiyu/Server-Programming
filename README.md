# Network

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

## SpinLock
* 스핀락이라는 이름은 락을 획득할 때까지 해당 스레드가 빙빙 돌고 있다(spinning)는 것을 의미
* 다른 쓰레드가 lock을 소유중이면 lock이 반환될 때까지 계속 확인하며 기다린다.
* 다른 쓰레드가 lock하는 시간이 짧을 때 유용하다. 루프를 돌면서 lock할 타이밍을 체크하므로 불필요한 컨텍스트 스위칭을 막을 수 있다.
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
	

