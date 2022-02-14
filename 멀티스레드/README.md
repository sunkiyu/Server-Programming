## 멀티쓰레드 
* <windows.h>에 있는 CreateThread는 윈도우에 종속적. 
* C++ 11에서 지원하는 thread를 include하여 std::thread를 사용하는 것이 범용성에 좋다.
* join() 함수로 쓰레드가 끝날때까지 기다린 후 종료한다. vector<>에 thread를 여러개 넣어놓고 관리할 수 있다.
* 전역변수 동기화 문제
> 전역변수 값 변경시 레지스트리와 메모리 사이에 읽기 쓰기 하는 과정 중    
  다른 쓰레드도 동시에 작업을 하기 때문에 덮어쓰기 문제가 발생하여 원하는 값이 나오지 않을 수 있다.   
  interlockedincrement 함수를 사용하거나 atomic을 include 하여 atomic<int32>와 같이 사용하면 해당 문제를 해결할 수 있다.       
  ->속도 저하 문제가 생길 수 있으므로 꼭 필요한 경우에만 사용하자   
  
#include <mutex>

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
