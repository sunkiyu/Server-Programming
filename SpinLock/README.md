## SpinLock
* 스핀락이라는 이름은 락을 획득할 때까지 해당 스레드가 빙빙 돌고 있다(spinning)는 것을 의미
* 다른 쓰레드가 lock을 소유중이면 lock이 반환될 때까지 계속 확인하며 기다린다.
* 다른 쓰레드가 lock하는 시간이 짧을 때 유용하다. 루프를 돌면서 CPU를 양보하지 않으므로 불필요한 컨텍스트 스위칭을 막을 수 있다.
* 다른 쓰레드가 lock하는 시간이 길 경우 루프로 인해 CPU 점유율이 높아질 수 있다.(Busy Waiting)   
* ## Thread 1이 먼저 공유 변수를 수정 중일 경우 Thread2가 lock() 함수에 진입한 상태를 나타낸 그림  
![image](https://user-images.githubusercontent.com/68372094/154225446-45e76c83-10d7-4ddc-b2a3-541869be6f79.png)   
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
