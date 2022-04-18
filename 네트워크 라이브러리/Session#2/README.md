# Session#2

![image](https://user-images.githubusercontent.com/68372094/162751344-823a4010-ab74-485d-936d-069bdc0b26ff.png)

1. 서버가 AcceptEx 호출
2. 클라이언트 접속
3. AccepEx GQCS 완료 통지 및 AcceptEx 완료 작업 처리
4. 클라이언트가 서버가 접속하여 수립된 세션의 소켓으로 WSARecv하여 클라이언트의 정보를 받을 준비를 한다.

* 앞의 과정은 Session#1 까지의 과정이다.
* 이 때 클라이언트에서 서버에 어떤 정보를 보내면 서버의 WSARecv 비동기 호출에 대한 완료 통지가 Worker Thread가 잠들어 있는 GQCS를 깨운다.
* GQCS가 깨어나면 AcceptEx 처리과정과 마찬가지로 OVERLAPPED 인자의 owner을 검사하게 되며 WSARecv의 경우 owner은 WSARecv를 호출한 Session이 된다.
* 따라서 Session의 Dispatch를 수행한다.
* WSARecv 호출시 인자로 넘겨주었던 recvBuffer을 통해 수신된 데이터를 읽을 수 있으며 echo 서버 작성시 받은 데이터를 그대로 Send 한다.
* 서버가 클라이언트로 Send 시 OVERLAPPED 구조체를 상속받은 SendEvent를 해당 Session 포인터를 담은 후 WSASend 비동기 호출하며 인자로 넘겨준다.
