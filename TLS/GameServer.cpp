#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

//windows 용 tls
_declspec(thread) int32 value;
//c++ 11
//쓰레드마다 자기만의 공간을 갖는다.
//thread_local 이 없을 경우 Data Race가 일어나 덮어써진다.
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
