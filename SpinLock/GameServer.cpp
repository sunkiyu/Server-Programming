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
		bool expected=false;
		bool desired = true;

		//CAS 의사코드
		//if (_locked == expected)
		//{
		//	expected = _locked;
		//	_locked = desired;
		//	//return true;
		//}
		//else
		//{
		//	expected = _locked;
		//	//return false;
		//}
		//_locked가 expected이면 desired로 바꿔라
		//spin 락이 계속 상태 체크를 하기때문에 cpu 점유율이 높아 질 수 있다. (상대 Lock이 오래 걸릴 경우)
		while(_locked.compare_exchange_strong(expected, desired) == false)
		{
			expected = false;
		}
		//while (_locked)
		//{
		//	_locked.store(false);
		//}

		//_locked = true;
	}

	void unlock()
	{
		//_locked = false;
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
