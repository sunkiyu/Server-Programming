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
