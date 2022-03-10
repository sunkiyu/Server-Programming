#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>
#include "ConcurrentQueue.h"
#include "ConcurrentStack.h"

LockQueue<int32> q;
LockFreeStack<int32> s;

void Push()
{
	while (1)
	{
		int32 value = rand() % 10;
		s.Push(value);

		//this_thread::sleep_for(10ms);
	}
}

void Pop()
{
	while (1)
	{
		
		int32 data;
		//waitPop도 가능
		if (s.TryPop(data))
		{
			cout << data << endl;
		}

	}
}

int main()
{
	thread t1(Push);
	thread t2(Pop);
	thread t3(Pop);

	t1.join();
	t2.join();
	t3.join();
}
