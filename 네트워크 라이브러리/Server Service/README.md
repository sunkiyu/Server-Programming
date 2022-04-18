# Server Service
![image](https://user-images.githubusercontent.com/68372094/162748012-00bda5b3-e381-4392-86dd-868d6428b525.png)

* ServerService에서는 IOCPCore와 Listener에 대한 포인터를 가지고 있어 상호 접근 가능하다.
* Worker Thread 들이 GetQueuedCompletionStatus(GQCS)를 호출하며 I/O 완료 작업이 발생하기를 기다리고 있다.
* AcceptEx에 대한 비동기 작업이 완료되면 GQCS에서 잠자던 WorkerThread가 깨어나 완료 작업을 수행한다.
* GQCS에서는 OVERLAPPED 구조체를 상속받아 AcceptEx 작업 시 인자로 넘겨준 AcceptEvent 를 복원할 수 있다.
* AcceptEvent 인자에는 Listener에 대한 정보와 Session에 대한 정보가 들어있으며 GQCS에서 복원하여 사용할 수 있다.
