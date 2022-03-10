## 가시성과 코드 재배치
* CPU 처리 순서 
> 1. Fetch 
> 2. Decode 
> 3. Excecute 
> 4. Write-Back

* 컴파일러와 CPU가 최적화를 위해 가시성과 코드 재배치를 수행하여 의도하는 값과 다른 값이 나올 수 있다.
* 가시성 - 멀티 코어 환경에서는 캐시와 레지스트리가 여러개 존재하여 멀티쓰레드가 공유 변수에 접근할 때 서로 다른 캐시와 레지스터에 읽고/쓰기 할 수 있는데
	  이 때 시간차이와 DataRace등 경합 상태가 발생하여 쓰레드 별로 서로 다른 값을 바라 볼 수 있다.
* 코드 재배치 - 컴파일러와 CPU가 효율적인 작업이라고 판단하여 최적화 시켜 코드 재배치가 일어날 수 있는데 이로 인해 의도하는 값과 다른 값이 나올 수 있다.
* 코드 재배치의 예
```cpp
int32 x = 0,y = 0,r1 = 0,r2 = 0;

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
	//r1 == 0 && r2 == 0인 상황이 나올 수 없는 구조지만 코드 재배치를 통해 
	//r1 == 0 && r2 == 0인 상황이 발생함
	cout << count << "-r1==0 && r2==0" << endl;
}
```
