
#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>

class SpinLock
{
public:
	void lock()
	{
		//CAS (compare and swap)
		bool expected = false;
		bool desired = true;

		while (_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
			//Sleep을 하면서 While문의 CPU 부하를 줄일 수 있다.
			//cout도 시간이 오래 걸리는 작업
			//커널 모드로 들어가는 것이 Context Switching 을 유발한다.
			//this_thread::sleep_for(std::chrono::milliseconds(100));			
			//this_thread::sleep_for(0ms);
			this_thread::yield();//현재 타임 슬라이스를 반환, 위의 sleep_for(0)과 같다.
		}
	}

	void unlock()
	{
		//_locked = false;
		//_locked를 false 시켜줘야 expected == _locked를 만족하여 lock할 수 있다.
		_locked.store(false);
	}
private:
	//atomic이 volatile 키워드 대체가능 기능 내포하고 있다.
	atomic<bool> _locked = false;
};

SpinLock spinLock;
int32 sum = 0;
mutex m;

void Add()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum++;
	}
}

void Sub()
{
	for (int32 i = 0; i < 10'0000; i++)
	{
		lock_guard<SpinLock> guard(spinLock);
		sum--;
	}
}

int main()
{

	thread t1(Add);
	thread t2(Sub);

	t1.join();
	t2.join();

	cout << sum << endl;
}
