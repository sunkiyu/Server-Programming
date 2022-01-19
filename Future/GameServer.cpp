#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

int64 ret;
int64 Calcualte()
{
	int64 sum = 0;
	for (int32 i = 0; i < 100'000; i++)
	{
		sum += i;
	}
	
	return sum;
}

void PromiseWorker(std::promise<string> && promise)
{
	promise.set_value("secret message");
}

void TaskWorker(std::packaged_task<int64(void)> &&task)
{
	task();
}

int main()
{
	//동기 실행
	int64 sum = Calcualte();
	cout << sum << endl;

	//std::future //단기 알바 쓰레드보다 가볍다.
	//비동기 실행
	{
		//1)deferred ->lazy evaluation 지연해서 실행
		//2)async ->별도의 쓰레드를 만들어서 실행
		//3)deferred | async ->알아서 실행 둘중에.
		//서버가 다른일감을 처리할때 future객체로 만들어서 여력이 될때 처리한다. 실행지연 비동기방식, 동기방식은 호출순간 실행한다.
		//async일 경우 별도의 쓰레드를 생성 
		//언젠가 미래에 결과물을 낼 것이다.
		std::future<int64> future = std::async(std::launch::async, Calcualte);

		//TODO
		//wait_for을 무한정 기다리는 것과 같다.
		future.wait();
		std::future_status status = future.wait_for(1ms);
		if (status == future_status::ready)
		{

		}
		int64 sum = future.get(); //결과물이 이제서야 필요하다.!

		class Knight
		{
		public:
			int64 GetHp() { return 100; }
		};
		Knight nite;

		//std::future<int64> ft = std::async(std::launch::async, Knight::GetHp, nite);//knight.GetHp();
	}

	//std::promise
	{
		//미래(std::future)에 결과물을 반환해줄거라 약속
		std::promise<string> promise;
		std::future<string> future = promise.get_future();

		thread t(PromiseWorker, std::move(promise));

		string message = future.get();
		//cout << message << endl;
		t.join();
	}

	//std::packaged_task
	{
		std::packaged_task<int64(void)> task(Calcualte);
		std::future<int64> future = task.get_future();

		std::thread t(TaskWorker, std::move(task));
		int64 sum = future.get();
		cout << sum << endl;
		t.join();
	}

	//결론 mutex,condition_variable까지 가지 않고 단순한 로직을 처리할 수 있음
	//일회성 이벤트에 유용, 비용이 싸다.
	//async
	//원하는 함수를 비동기적으로 실행
	//promise
	//결과물을 promise를 통해 future로 받아줌
	//packaged_task
	//원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌
	//promise Packaged_task 사용할일 별로 없음
	//async는 종종 사용
}
