#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <future>

atomic<bool> flag = false;
atomic<bool> ready;
int32 value;

/*
void Producer()
{
	value = 10;
	ready.store(true, memory_order::memory_order_seq_cst);
	//memory_order_relaxed는 위아래 코드 순서가 바뀔수있다. 거의 사용X
};
*/

/*
void Producer()
{
ready.store(true, memory_order::memory_order_relaxed);
value = 10;
//memory_order_relaxed는 위아래 코드 순서가 바뀔수있다. 거의 사용X
};
*/
//
//void Producer()
//{
//	value = 10;
//	ready.store(true, memory_order::memory_order_release);
//	std::_Atomic_thread_fence(memory_order_release); //메모리 가시성 보장 재배치 금치 //실전 사용 거의 없다.
//	//절취선---------------------------------------------
//};
//
//void Consumer()
//{
//	//절취선---------------------------------------------
//	while (ready.load(memory_order::memory_order_acquire) == false) {};
//	cout << value << endl;
//};

//디폴트로 사용했어도 문제가 없다. seq_cst 버전
void Producer()
{
	value = 10;
	ready.store(true);
};

void Consumer()
{
	while (ready.load() == false) {};
	cout << value << endl;
};

//void Consumer()
//{
//	while (ready.load(memory_order::memory_order_seq_cst) == false) {};
//	cout << value << endl;
//};

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);
	t1.join();
	t2.join();

	//true =>원래 원자적으로 처리되었다. 
	//false 원자적 처리가 필요하다.
	flag.is_lock_free();
	flag = true;
	bool val = flag;

	flag.store(true,memory_order::memory_order_seq_cst);
	flag.load(memory_order::memory_order_seq_cst);

	//이전 flag 값을 prev에 넣고 flag값을 수정
	{
		//flag 값을 prev에 넣기 전에 다른 쓰레드가 수정할 수 있다.
		//이전 값을 넣고 true로 바꾸겠다. 아토믹한 연산
		bool prev = flag.exchange(true);
		//bool prev = flag;
		//flag = true;
	}

	//CAS(compare and swap) 조건부 수정
	{
		//expected 예상한값. flag와 expected 가 같은 값인가? 그럼 desired로 바꾼다.
		bool expected = false;
		bool desired = true;
		flag.compare_exchange_strong(expected, desired);

		//compare_exchange_strong의 원자적 연산과정 요약
		if (flag == expected)
		{
			flag = desired;
			return true;
		}
		else
		{
			expected = flag;
			return false;
		}
	}
	{
		//compare_exchange_weak은 while문과 같이쓴다. 실패시 재시도
		while (1) {
			bool expected = false;
			bool desired = true;
			//다른 쓰레드의 interruption을 받아서 실패할 수 있음.
			//flag ==expected 여도 실패 할 수 있다.
			flag.compare_exchange_weak(expected, desired);
		}
	}

	//Memory model 정책
	//1.Sequentially Consistent(seq_cst)
	//2.Acquire-release(consume,acquire,release,acq_rel)
	//3.Relaxed(relaxed)

	//1.seq_cst(가장 엄격) 컴파일러 최적화 여지 적음 직관적, atomic 동작원리
	//가시성 코드재배치 바로 해결



	//2.acquire_release 핵심
	//딱 중간
	//release 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지
	//acquire 로 같은 변수를 읽는 스레드가 있다면 release 이전의 명령들이 acquire하는 순간에 관찰 가능 가시성 보장

	//consume 별로 안중요
	//4.relaxed(자유롭다) 컴파일러 최적화 여지 많음 직관적이지 않음
	//너무나도 자유롭다.
	//코드재배치 가시성 해결 못한다.
	//가장 기본 조건 동일 객체에 대한 동일 관전 순서만 보장

	//intel AMD의 경우 순차적 일관성을 보장하므로 seq_cst를 써도 별다른 부하 없음\
	//ARM의 경우 차이가 있다.
}
