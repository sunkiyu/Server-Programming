# Session3

![image](https://user-images.githubusercontent.com/68372094/162753201-5ff30a0a-5232-42e4-bb89-aff45e5a241c.png)

* 클라이언트가 서버에 접속하기 위해 최대 세션 개수 만큼 세션을 생성한다.
* Session의 ConnectEvent 멤버의 owner에 자기 자신에 대한 포인터를 넣고 EventType 인자에 Connect를 넣는다.
* ConnectEx 비동기 연결 함수를 호출하며 ConnectEvent를 인자로 넘긴다.
* 클라이언트도 서버와 대칭적으로 Worker Thread 들이 GetQueuedCompletionStatus를 호출하며 비동기 완료 통지가 오기만을 기다린다.
* GQCS 완료 통지가 오면 OVERLAPPED 인자를 복원 시키며 owner을 확인하여 Dispath 작업을 수행한다.
* 복원된 OVERLAPPED 인자의 EventType이 Connect이므로 ProcessConnect를 수행한다.
* ProcessConnect로 서버로 데이터를 WSASend하고 다시 WSARecv 한다.
* 서버가 Echo 서버라면 클라이언트가 보낸 데이터를 그대로 다시 클라이언트로 보내주기 때문에 클라이언트와 데이터를 서로 핑퐁하며 주고 받는다.
