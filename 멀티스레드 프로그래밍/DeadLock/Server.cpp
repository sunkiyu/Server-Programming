// GameServer.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//
#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <mutex>
//c++ 11
#include <thread> // 윈도우 리눅스 범용적

#include "AccountManager.h"
#include "UserManager.h"

void Func1()
{
	for (int32 i = 0; i < 1000; i++)
	{
		UserManager::instance()->ProcessSave();
	}
}

void Func2()
{
	for (int32 i = 0; i < 1000; i++)
	{
		AccountManager::instance()->ProcessLogin();
	}
}

int main()
{
	std::thread t1(Func1);
	std::thread t2(Func2);

	t1.join();
	t2.join();

	cout << "jobs Done" << endl;
}

