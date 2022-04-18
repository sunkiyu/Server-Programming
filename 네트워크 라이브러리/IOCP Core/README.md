## IOCP Core
![image](https://user-images.githubusercontent.com/68372094/161079775-2afa1b15-572d-4ed8-8124-49f47ce5c7ac.png)

* Listener 자신(Key)과 소켓(Handle)을 IOCP에 등록한다. 
* 이렇게하면 Listener 소켓에 대한 비동기 I/O 작업 완료 알림을 받을 수 있다.
* 비동기 AcceptEx 시 OVERLAPPED 구조체를 상속받은 AcceptEvent를 OVERLAPPED 인자에 넘겨주면 getqueuedcompletionstatus(GQCS) 시 OVERLAPPED 인자보다 더 많은 정보를 얻을 수 있다.
* 해당 프로젝트에서는 AcceptEvent에 Session 포인터를 물려주어 GQCS 시 OVERLAPPED 인자에서 AcceptEvent 인자를 복원하면서 Session에 대한 정보도 함께 얻을 수 있도록 하였다.
