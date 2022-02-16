# Server Programming
* * *
# 목차
 * [서버란](#서버란)
  * [멀티스레드](https://github.com/sunkiyu/Server-Programming/blob/560314354b3ed3e3724ecb1b0e97159afe3c0153/%EB%A9%80%ED%8B%B0%EC%8A%A4%EB%A0%88%EB%93%9C/README.md)
  * [DeadLock](https://github.com/sunkiyu/Server-Programming/blob/d6cbc3107258c8e51043898bc4650f4cd4fed0e0/DeadLock/README.md)
  * [SpinLock](https://github.com/sunkiyu/Server-Programming/blob/6fa75ba695d49cbe63dca9ffe5bb223281aad149/SpinLock/README.md)
  * [CAS(Compare And Swap)](https://github.com/sunkiyu/Server-Programming/blob/943644d872001ca56861b23563f1a9a61ca69c9b/CAS/README.md)
  * [Sleep](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/Sleep/README.md)
  * [Event](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/Event/README.md)
  * [Condition Variable](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/Condition%20Variable/README.md)
  * [Future](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/Future/README.md)
  * [Cache](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/Cache/README.md)
  * [가시성/코드 재배치](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/%EA%B0%80%EC%8B%9C%EC%84%B1-%EC%BD%94%EB%93%9C%20%EC%9E%AC%EB%B0%B0%EC%B9%98/README.md)
  * [메모리 모델](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/Memory%20Model/README.md)
  * [Thread Local Stoage(TLS)](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/TLS/README.md)
  * [Lock-Based Stack](https://github.com/sunkiyu/Server-Programming/blob/33423e51a5929be9d3a938a6b7e2439d84f42ecc/LockBased%20Stack%20Queue/README.md)
  * [Lock-Based Queue](https://github.com/sunkiyu/Server-Programming/blob/39b0bcae6e48a495601073a1005a9830f67982fa/LockBased%20Stack%20Queue/README.md)
  * [Lock-Free-Stack#1](https://github.com/sunkiyu/Server-Programming/blob/e8e1eb5fe53f8574cbf172b9bd42dd933f99f949/Lock-Free-Stack%231/README.md)
  * [Lock-Free-Stack#2](https://github.com/sunkiyu/Server-Programming/blob/e8e1eb5fe53f8574cbf172b9bd42dd933f99f949/Lock-Free-Stack%232/README.md)
  * [ThreadManager](https://github.com/sunkiyu/Server-Programming/blob/ce48ed5016838b434bd97efeac10f0daec92b81d/ThreadManager/README.md)
  * [Reader-Writer-Lock](https://github.com/sunkiyu/Server-Programming/blob/bf6b74ded550721db0617aac70b4854fd519cd21/Reader-Writer-Lock/README.md)
  * [Dead-Lock 탐지](https://github.com/sunkiyu/Server-Programming/blob/9648d53a8f23a825ffde403be6ee80bd6196f06c/DeadLock%20Detection/README.md)
  * [멀티스레드 소수구하기 문제](https://github.com/sunkiyu/Server-Programming/blob/273667fa034ad9ce06a50c8f0608ff9f5abf1922/%EB%A9%80%ED%8B%B0%EC%8A%A4%EB%A0%88%EB%93%9C%20%EC%86%8C%EC%88%98%20%EA%B5%AC%ED%95%98%EA%B8%B0/README.md)
  * [레퍼런스 카운팅](https://github.com/sunkiyu/Server-Programming/blob/09b55dc4d0b8d82f16310322fa867d9d2859baf7/ReferenceCounting/README.md)
  * [Allocator](https://github.com/sunkiyu/Server-Programming/blob/16dff2a85aa74d43cfc077488d2772c43038050b/Allocator/README.md)
  * [Stomp Allocator](https://github.com/sunkiyu/Server-Programming/blob/c2558c16d9c5db573dde237ba207069131d105a3/Stomp-Allocator/README.md)
  * [Memory Poo1#1](https://github.com/sunkiyu/Server-Programming/blob/56ca3cdae840ef097fa485dbea188e92f5e61a7e/Memory%20Pool%231/README.md)
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
