#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <windows.h>

mutex m;
queue<int32> q;
HANDLE handle;

//참고) CV는 User-Level Object(커널 오브젝트 x)
condition_variable cv;

void Producer()
{
	while (true)
	{
		//1) Lock을 잡고
		//2) 공유 변수 값을 수정
		//3) Lock을 풀고
		//4) 조건 변수 통해 다른 쓰레드에게 통지
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}
		cv.notify_one();//wait중인 쓰레드가 있으면 1개를 깨운다.
		//::SetEvent(handle);
		//this_thread::sleep_for(100000000ms);
	}
};

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		cv.wait(lock, []() {return q.empty() == false; });
		//1) Lock을 잡고
		//2) 조건 확인
		//만족 ->빠져나옴, 만족 x-> Lock을 풀고 대기상태

		//while (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
};

int main()
{
	//이벤트는 커널에서 만들어준다.
	//이벤트는 커널 오브젝트
	//Usage Count
	//Signal, Non-Signal
	//Auto Reset, Manual Reset
	handle = ::CreateEvent(NULL,/*보안속성*/FALSE/*bManualRest*/, FALSE/*bInitialState*/, NULL);
	
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	::CloseHandle(handle);

}
