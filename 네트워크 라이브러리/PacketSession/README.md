# PacketSession

![image](https://user-images.githubusercontent.com/68372094/163186488-e15b7f25-c53b-4010-a4d1-551d57f68469.png)

* 서버와 클라이언트가 주고 받는 패킷에 PacketHeader구조체에 맞게 패킷 사이즈 전체와 패킷 아이디를 채운 뒤 바로뒤에 데이터를 입력한다.
* 따라서 서버에서 SendBufferChunk를 통해 Header에 사이즈와 패킷을 채우고 데이터를 채워 별도의 복사 작업없이 BroadCast로 세션에 연결된 모든 클라이언트에게 데이터를 보낼 수 있다.
* 클라이언트 쪽에서는 받은 데이터를 PacketHeader 구조체 포인터로 파싱하면 ID와 패킷 사이즈를 얻을 수 있다.
* 패킷 사이즈 - 패킷 헤더 사이즈가 데이터 사이즈이므로 데이터 사이즈를 구해 읽어온다.
