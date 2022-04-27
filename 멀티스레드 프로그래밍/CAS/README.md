## CAS  
-compare_exchange_strong
-compare_exchange_weak   
=>두 함수가 모두 atomic 한 CAS 작업을 제공한다.   
* weak과 strong의 가장 큰 차이점은 weak 같은 경우 교환 가능하더라도 CAS 작업이 실패 할 수 있고, strong의 경우 교환 가능하면 항상 성공한다.   
* 대부분의 경우에는 Loop + weak이 좋다.   
* spurious failure을 허용하지 않는 경우 strong을 쓴다.   
```cpp
#include<iostream>
#include<thread>

int sum = 0;

void Add()
{
    for (int i = 0; i < 100000000; i++)
    {
        sum++;
    }
}

int main(int argc, char *argv[])
{
    std::thread t1(Add);
    std::thread t2(Add);

    t1.join();
    t2.join();

    std::cout << sum << std::endl;
    return 0;
}
```   
위의 코드를 실행하면 원칙적으로는 200000000이 출력되어야한다.   
sum이 thread1과 thread2를 통해 각각 100000000 X 1번씩 증가하였기 때문이다.   
하지만 결과는?   
![image](https://user-images.githubusercontent.com/68372094/151298276-2b581cad-fcb9-4cbc-bee3-60bc4fcc5ab8.png)   
위와 같이 200000000과는 전혀 다른 엉뚱한 값이 나오게 된다.   
sum이 공유 변수이기 때문에 멀티스레드 환경에서 DataRace 환경에 처했기 때문이다.   
sum이 1 증가할 때 생각보다 많은 작업이 일어나는데   
1. 메모리에서 sum의 값을 레지스터로 fetch 한다.   
2. CPU ALU에서 증가 연산   
3. 메모리에 증가된 값을 저장   
1부터 3까지의 과정이 원자적으로 *한번에 하나의 스레드에 의해 처리되어야* 하지만 멀티스레드 환경에서는 1~3 과정 중 다른 스레드가 침범하여 공유 변수값을 오염시킬 우려가 있다.   
흔히 함수 호출 단위로 CPU가 명령어를 처리하는 것으로 생각하지만 단순한 cout 호출시에도 Context Switching 과정이 일어나기 때문에 의도와 다른 값이 나올 수 있다.
따라서 1부터 3까지의 과정을 원자적으로 처리해야 한다.   
### Thread 1이 먼저 공유 변수를 수정 중일 경우 Thread2가 lock() 함수에 진입한 상태를 나타낸 그림  
![image](https://user-images.githubusercontent.com/68372094/154225446-45e76c83-10d7-4ddc-b2a3-541869be6f79.png)   
```cpp   
#include<iostream>
#include<queue>
#include<mutex>
#include<thread>
#include<atomic>

int sum = 0;
std::mutex m;
std::atomic<bool> _locked= {false};

void Add()
{
    for (int i = 0; i < 100000000; i++)
    {
        bool expected=false;
        bool desired = true;

        while(_locked.compare_exchange_strong(expected, desired) == false)
        {
            expected = false;
        }
        sum++;
         _locked.store(false);
    }
}
				  
int main(int argc, char *argv[])
{
    std::thread t1(Add);
    std::thread t2(Add);

    t1.join();
    t2.join();

    std::cout << sum << std::endl;
    return 0;
}
```    
![image](https://user-images.githubusercontent.com/68372094/151299019-cd5a4ffd-0398-4ff4-8e65-b43c8599d44a.png)   
위와 같이 CAS (Compare And Swap)을 적용하면 1~3의 과정을 원자적으로 처리하여 우리가 의도한 값 200000000을 얻을 수 있다.   
