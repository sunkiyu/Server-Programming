## DeadLock
* Lock 순서를 지켜준다.   
	=>A쓰레드에서 mutexA -> mutexB 순서로 Lock을 했을 경우 B쓰레드도 mutexA->mutexB 순서로 Lock을 한다.
* 개발단계에서는 일어나지 않다가 배포 후 다수의 사용자가 사용할 경우 간혹가다가 일어나는 경우가 대다수다.   
![image](https://user-images.githubusercontent.com/68372094/154211715-d8c52185-5d59-4b0d-ad15-2282e2b55f79.png)   
> 1번 함수와 2번 함수를 교착상태를 고려하지 않고 mutex lock을 무한히 진행 하다보면 언젠간 DeadLock 상태에 빠지게 된다.   
> 1번 함수 호출로 후 mutexA가 락이 잡히고 3번 함수를 호출하려는 순간(mutexU가 락이 걸리기전), 2번 함수에서 mutexU락을 걸어버리면 3번 함수 호출 후 mutexU락이 걸려 있어 
> 블로킹 상태가 되며, 4번 함수 역시 mutexA 락을 잡으려고 시도하지만 mutexA가 이미 락이 걸려 있기 때문에 블로킹 상태에 놓인다.
```cpp
mutex m1;
mutex m2;
std::lock(m1, m2); //교착상태 회피 알고리즘을 사용하여 lock 해준다.	
//adopt_lock 이미 lock 된 상태니까 나중에 풀어주기만 해라
//assume the calling thread already has ownership of the mutex
lock_guard<mutex> g1(m1, std::adopt_lock);
```
* lock_guard 객체는 Scope를 벗어날때 자동으로 unlock 시켜주기 때문에 데드락 현상 예방에 효과적이다.
* 알고리즘 그래프로 생각해볼 때 락의 사이클이 발생하면 데드락
* 유저레벨 커널레벨 컨텍스트 스위칭 부하 생각보다 크다. 	
### RAII(Resource Acquisition is Initialization)   
C++에서 객체가 생성되며 할당된 리소스가 Scope를 벗어나면 리소스를 해제해주는 설계 패턴.   
stack에 할당된 리소스는 Scope를 벗어나면서 소멸자가 호출되며 리소스가 해제되는 원리를 이용.   
new로 동적할당 받은 힙 메모리의 Resource는 프로그래머가 직접 관리해야 하지만 예외상황이난 exception 등의 이유로   
리소스 해제를 못하고 Memory Leak이 발생하거나 Mutex의 경우 lock/unlock을 제대로 수행하지 못해 교착상태에 빠질 수 있다.   
unique_ptr, shared_ptr, lock_guard 등으로 이러한 문제들을 예방할 수 있는데 Scope를 벗어나면 자동으로 리소스 해제를 시켜주기 때문이다.   
이렇게 함수 종료시 무조건 실행을 보장해주는 클래스들의 기능적인 부분들을 통틀어 RAII라고 부름
