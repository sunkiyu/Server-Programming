# RecvBuffer

![image](https://user-images.githubusercontent.com/68372094/157193180-17bef79c-cfc8-4b8b-b40a-d9bc2185790c.png)
![image](https://user-images.githubusercontent.com/68372094/162964423-1d6e3106-d571-4ff2-9c14-878e79f58a65.png)

* 서버에서 AcceptEx 작업 완료 후 클라이언트 정보를 받기 위해 WSARecv 작업을 하는데 이 때 클라이언트와 Accept한 세션의 RecvBuffer의 WritePos 위치를 인자로 넘긴다.
* 클라이언트의 정보를 수신하여 WSARecv에 대한 완료 통지가 GQCS에 오면 ProcessRecv 작업을 수행한다.
* GQCS를 통해 전송된 바이트 수 만큼 RecvBuffer의 WritePos 커서를 옮긴다.
* WritePos 커서 - ReadPos 커서 만큼 ReadPos로 부터 데이터를 읽는다. 이것이 클라이언트로 부터 수신된 데이터이다.
* ReadPos 커서도 읽은 만큼 뒤로 이동시킨다.
* ReadPos와 WritePos 커서가 일치 할 경우는 읽은 데이터가 없는 것이므로 커서를 맨 앞으로 이동시켜 버퍼를 재사용한다.
* ReadPos와 WritePos 가 일치하지 않고 현재 RecvBuffer의 남은 사이즈가 매우 작을 경우 불가피하게 ReadPos와 WritePos 커서 사이의 데이터를 복사하여 버퍼의 맨 앞으로 옮긴 후 커서 또한 옮겨준다. 
