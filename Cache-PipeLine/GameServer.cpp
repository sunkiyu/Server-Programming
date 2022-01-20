#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>


//가시성, 코드 재배치
//멀티쓰레드 환경에서는 원하는 값이 아닐 수 있다.
//r1,r2가 0인 상황이 나올 수 없다.
//컴파일러 혹은 cpu가 상황에 따라 코드를 재배치 할 수도 있다.(로직이 변하지 않는다고 생각할 때)

int32 x = 0;
int32 y = 0;
int32 r1 = 0;
int32 r2 = 0;

volatile bool ready;

void Thread_1()
{
	while (!ready);
	y = 1; //store y
	r1 = x; //x load
}
void Thread_2()
{
	while (!ready);
	x = 1; //store y
	r2 = y; //x load
}


int main()
{
	int32 count = 0;

	while (true)
	{
		ready = false;
		count++;
		x = y = r1 = r2 = 0;

		thread t1(Thread_1);
		thread t2(Thread_2);
		
		ready = true;

		t1.join();
		t2.join();

		if (r1 == 0 && r2 == 0)
			break;
	}

	cout << count << "번만에 빠져나옴" << endl;
}
