#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <mutex>
//c++ 11
#include <thread> // 윈도우 리눅스 범용적
//Mutual ExClusive(상호배타적)
//수동관리 힘들다.
//RAII (Resource Acquisition is Initialization)

template<typename T>
class LockGuard
{
public:
	LockGuard(T&m)
	{
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard()
	{
		_mutex->unlock();
	}
private:
	T *_mutex;
};

mutex m;
vector<int32> v;

void Push()
{
	//락가드 객체 자체가 소멸할때 unlock 해준다.
	std::lock_guard<std::mutex> lockGuard(m);
	for (int i = 0; i < 10000; i++)
	{
	    //재귀호출 안됨
		//LockGuard<std::mutex> lockGuard(m);
		//std::lock_guard와 동일
		//std::lock_guard<std::mutex> lockGuard(m);
		//std::unique_lock<std::mutex> uniqueLock(m, std::defer_lock);
		v.push_back(i);

		//m.unlock();
	}
}

int main()
{
	v.reserve(20000);
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}
