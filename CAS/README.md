## CAS  
-compare_exchange_strong
-compare_exchange_weak
=>두 함수가 모두 atomic 한 CAS 작업을 제공한다.
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
![image](https://user-images.githubusercontent.com/68372094/154224264-cba7e19e-3d58-4226-9b98-af0f40f08a80.png)   
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
원자적 처리 과정은 아래와 같다.   
1. 스레드1이 먼저 Add 함수의 루프에서 compare_exchange_strong 함수를 호출한다.
2. 스레드1이 최초로 compare_exchange_strong을 호출하였기 때문에 _locked와 expected는 모두 false를 나타낸다.   
3. 따라서 _locked는 desired 값으로 변경된 후(_locked == desired == true) true를 리턴하여 while문을 빠져 나온뒤 sum을 증가시키는 연산을 한다.
4. 이때 스레드2도 실행 중이므로 sum 공유 변수를 건드리려 하는데 스레드 2가 compare_exchange_strong를 호출한 시점은   
   3번 과정에서 _locked== true, expected==false 이므로 expected = _locked 값으로 수정한 뒤 false를 리턴하고 _locked(true)로 수정된 expected를 false로 수정한다.   
5. false를 리턴하였으므로 다시 while 문에서 compare_exchange_strong을 호출하는데 4번과 마찬가지 상태이므로 while 루프에 머물면서 sum 공유 변수를 건드리지 못한다.
6. sum 증가 연산을 한뒤 비로소 _locked.store ==false를 호출하는데 이때 while문에 머물던 스레드2 가 _locked== false, expected == false 상태로 true를 리턴하고 sum   
   공유변수의 값을 증가시킬 기회를 얻는다.
7. 위와 같은 과정이 두 스레드 모두 루프를 돌 때까지 반복된다.  
