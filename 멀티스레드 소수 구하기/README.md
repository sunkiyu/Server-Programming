## 멀티스레드 소수 구하기   
```cpp
#include <atomic>
#include <windows.h>
#include <iostream>
#include <thread>
using namespace std;
atomic_int ssAtomic;

void ssCount(int start,int end)
{
    for(int num =start; num<=end; num++)
    {
        if(num==1){
            continue;
        }
        bool isss= true;
        for(int i=2; i<num; i++)
        {
            if(!(num%i)){
                isss = false;
                break;
            };
        }
        if(!isss)continue;
        ssAtomic++;
    }
}

int main(int argc, char *argv[])
{
   int start = GetTickCount();
   thread t1(ssCount,1,300000);
   t1.join();
   int end = GetTickCount();
   
   cout<<"Total= "<<ssAtomic<<" "<<end-start<<"ms"<<endl;
   return 0;
}
```   
![image](https://user-images.githubusercontent.com/68372094/152285760-ffc3039e-8525-4565-bc6b-a03eaf32f0fb.png)   
싱글스레드 환경에서는 300,000까지 소수의 개수를 구하는데 15875ms(약15초)의 시간이 소요된다.      
```cpp
#include <windows.h>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;
atomic_int ssAtomic;

void ssCount(int start,int end)
{
    for(int num =start; num<=end; num++)
    {
        if(num==1){
            continue;
        }
        bool isss= true;
        for(int i=2; i<num; i++)
        {
            if(!(num%i)){
                isss = false;
                break;
            };
        }
        if(!isss)continue;
        ssAtomic++;
    }
}

int main(int argc, char *argv[])
{
   int start = GetTickCount();
   vector<thread> threads;
   int coreCnt = thread::hardware_concurrency();
   
	threads.push_back(thread(ssCount,1,70000));
	threads.push_back(thread(ssCount,70001,140000));
	threads.push_back(thread(ssCount,140001,210000));
	threads.push_back(thread(ssCount,210001,300000));
	 
   for(int i=0; i<4; i++)
   {
   	threads[i].join();
   }
   int end = GetTickCount();
   
   cout<<"Total= "<<ssAtomic<<" "<<end-start<<"ms"<<endl;
   return 0;
}
```
![image](https://user-images.githubusercontent.com/68372094/152287019-2b2ddfc7-e8c8-4377-9fde-fee25cf7d056.png)   
hardware_concurrency를 통해 PC코어 개수를 확인해보니 4개였다. 따라서 스레드를 4개 이상 생성하는 것은 이 프로젝트에서 큰 의미가 없어보이므로 스레드 4개를 생성후 실험하였다.   
9087ms(약 9초)로 6초 가량 시간이 단축되는 것을 확인 할 수 있었다.   
![image](https://user-images.githubusercontent.com/68372094/152288482-1b3e1993-226c-4efb-8627-e4817dca3f05.png)   
4코어를 모두 높은 사용률을 나타낸다. 4개 코어가 4개의 스레드를 담당하기 때문   
![image](https://user-images.githubusercontent.com/68372094/152288999-4d71e802-b26b-48e7-9cab-53513d76a97a.png)   
싱글스레드일 경우 컨텍스트 스위칭을 통해 CPU부하를 나누기 때문에 부하가 덜하다. 4개의 코어가 1개의 스레드를 담당   
