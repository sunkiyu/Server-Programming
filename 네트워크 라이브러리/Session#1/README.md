## Session#1 개요
![image](https://user-images.githubusercontent.com/68372094/163758902-a4e0def0-a823-4490-811c-6429a7736459.png)

* 서버가 처리할 최대 세션 수 만큼 AcceptEvent와 Session을 생성하고 AcceptEvent가 Session에 대한 정보를 갖고 있게한다.
* 각각의 생성된 세션 수 만큼 AcceptEx를 호출하여 클라이언트의 접속이 왔을 때 완료 통지가 되도록 하고 있다.
* WorkerThread 들이 IOCP에 등록된 Listener 소켓 완료 작업 검사를 수행한다. 완료통지가 아직 오지 않았을 경우 계속 대기한다.
* 만약 클라이언트가 서버에 접속하여 완료 통지가 되면  GQCS가 깨어나고 OVERLAPPED 인자를 통해 owner를 검사한다.
* AcceptEx의 경우 owner가 Listener이므로 Lister에 있는 Dispatch 함수를 호출하여 OVERLAPPED 인자로 전달받은 값을 AccpetEvent로 캐스팅하여 해당 Session 및 클라이언트 소켓에 대한 정보를 얻는다.
* 그 후 클라이언트에서 오는 정보를 받기 위해 OVERLAPPED 구조체를 상속받은 RecvEvent에 Session에 대한 정보를 넣은 후 해당 Session의 소켓으로 WSARecv 비동기 함수를 호출하며 RecvEvent 인자와 RecvBuffer을 넘긴다.
* WSARecv 작업 역시 I/O 작업 완료시 GQCS 를 통해 완료 통지가 오면 처리한다.

