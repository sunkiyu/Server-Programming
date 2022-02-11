# Server Programming
* * *
# 목차
 * [서버란](#서버란)
  * [멀티쓰레드](#멀티쓰레드)
  * [DeadLock](https://github.com/sunkiyu/Server-Programming/blob/ea5804437b3fc1b0bc5c0e74fe50ab0eae16b99e/DeadLock/README.md)
  * [SpinLock](https://github.com/sunkiyu/Server-Programming/blob/514193b66bb37ae9b7a6032012482cda0f6a470c/SpinLock/README.md)
  * [CAS(Compare And Swap)](https://github.com/sunkiyu/Server-Programming/blob/083f88889266a8e1e7d28d7cb19140e4b39d4522/CAS/README.md)
  * [Sleep](https://github.com/sunkiyu/Server-Programming/blob/35042c521d923b0afc54bfb8b4fef260164ccf2e/Sleep/README.md)
  * [Event](https://github.com/sunkiyu/Server-Programming/blob/865fdadd86661e9e183d2dd3c91d7b3d6b65f9d6/Event/README.md)
  * [Condition Variable](https://github.com/sunkiyu/Server-Programming/blob/e03faf4da32ef2e15b9d35849c2b5c1c5484db15/Condition%20Variable/README.md)
  * [Future](https://github.com/sunkiyu/Server-Programming/blob/bb8922b00295ba37977bdfd6eefc5149f21121d7/Future/README.md)
  * [Cache](https://github.com/sunkiyu/Server-Programming/blob/25ee0f9553e0666642a3705734ce01137a60ccac/Cache/README.md)
  * [가시성/코드 재배치](https://github.com/sunkiyu/Server-Programming/blob/1d09a1dc94c99a6e2145a2052d3aebb79dbe67c6/%EA%B0%80%EC%8B%9C%EC%84%B1-%EC%BD%94%EB%93%9C%20%EC%9E%AC%EB%B0%B0%EC%B9%98/README.md)
  * [메모리 모델](https://github.com/sunkiyu/Server-Programming/blob/1daf24d651a6544c348e586dde454bfeedcd97ef/Memory%20Model/README.md)
  * [Thread Local Stoage(TLS)](https://github.com/sunkiyu/Server-Programming/blob/bdb74293bc6638e5ffad988c3ed7065658f5d692/TLS/README.md)
  * [Lock-Based Stack](https://github.com/sunkiyu/Server-Programming/blob/39b0bcae6e48a495601073a1005a9830f67982fa/LockBased%20Stack%20Queue/README.md)
  * [Lock-Based Queue](https://github.com/sunkiyu/Server-Programming/blob/39b0bcae6e48a495601073a1005a9830f67982fa/LockBased%20Stack%20Queue/README.md)
  * [Lock-Free-Stack#1](https://github.com/sunkiyu/Server-Programming/blob/e8e1eb5fe53f8574cbf172b9bd42dd933f99f949/Lock-Free-Stack%231/README.md)
  * [Lock-Free-Stack#2](https://github.com/sunkiyu/Server-Programming/blob/e8e1eb5fe53f8574cbf172b9bd42dd933f99f949/Lock-Free-Stack%232/README.md)
  * [ThreadManager](https://github.com/sunkiyu/Server-Programming/blob/ce48ed5016838b434bd97efeac10f0daec92b81d/ThreadManager/README.md)
  * [Reader-Writer-Lock](https://github.com/sunkiyu/Server-Programming/blob/bf6b74ded550721db0617aac70b4854fd519cd21/Reader-Writer-Lock/README.md)
  * [Dead-Lock 탐지](https://github.com/sunkiyu/Server-Programming/blob/9648d53a8f23a825ffde403be6ee80bd6196f06c/DeadLock%20Detection/README.md)
  * [멀티스레드 소수구하기 문제](https://github.com/sunkiyu/Server-Programming/blob/273667fa034ad9ce06a50c8f0608ff9f5abf1922/%EB%A9%80%ED%8B%B0%EC%8A%A4%EB%A0%88%EB%93%9C%20%EC%86%8C%EC%88%98%20%EA%B5%AC%ED%95%98%EA%B8%B0/README.md)
  * [레퍼런스 카운팅](https://github.com/sunkiyu/Server-Programming/blob/09b55dc4d0b8d82f16310322fa867d9d2859baf7/ReferenceCounting/README.md)
  * [Allocator](https://github.com/sunkiyu/Server-Programming/blob/690db615ae24be5a2564032546a218659f16bc9f/Allocator/README.md)
  * [Stomp Allocator](https://github.com/sunkiyu/Server-Programming/blob/c2558c16d9c5db573dde237ba207069131d105a3/Stomp-Allocator/README.md)
  * [Memory Poo1#1](https://github.com/sunkiyu/Server-Programming/blob/ea27799ed0c41ae6716d631317daf9d4f2e08539/Memory%20Pool%231/README.md)
* * *
## 서버란
-다른 컴퓨터에서 연결 가능하도록 상시 실행대기하며 서비스를 제공하는 프로그램

### Web Server(예 HTTP Server)
* 정보 요청/갱신이 적다
* 실시간 인터랙션이 없다.
* 웹 서비스를 만드는데 주로 사용(구글,네이버,아마존 등)
* 처음부터 만들지 않고 프레임웍을 하나 골라서 사용
  (Spring(Java), Node JS(JavaScript),vASP.net(C#),vPHP)

### Game Server(예 TCP Server, Binary Server)
* 요청/갱신 많다.
* 실시간 인터랙션 있다.
* 서버가 클라이언트에 질의 가능
* stateful(상태 갱신 및 교환이 많다)

### 게임서버 작성시 고려대상
* 최대 동시 접속자
* 게임 장르 및 채널링
* 게임 로직
* 쓰레드 개수
* 쓰레드 모델
* 네트워크 모델
* 반응성
* DB

## 멀티쓰레드 
* <windows.h>에 있는 CreateThread는 윈도우에 종속적. 
* C++ 11에서 지원하는 thread를 include하여 std::thread를 사용하는 것이 범용성에 좋다.
* join() 함수로 쓰레드가 끝날때까지 기다린 후 종료한다. vector<>에 thread를 여러개 넣어놓고 관리할 수 있다.
* 전역변수 동기화 문제
> 전역변수 값 변경시 레지스트리와 메모리 사이에 읽기 쓰기 하는 과정 중    
  다른 쓰레드도 동시에 작업을 하기 때문에 덮어쓰기 문제가 발생하여 원하는 값이 나오지 않을 수 있다.   
  interlockedincrement 함수를 사용하거나 atomic을 include 하여 atomic<int32>와 같이 사용하면 해당 문제를 해결할 수 있다.       
  ->속도 저하 문제가 생길 수 있으므로 꼭 필요한 경우에만 사용하자   

