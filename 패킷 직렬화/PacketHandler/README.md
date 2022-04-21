# PacketHandler

![image](https://user-images.githubusercontent.com/68372094/163393476-44891bdd-c7b1-42a2-8956-b9adf6906af2.png)

![image](https://user-images.githubusercontent.com/68372094/163393517-009eacdb-6a3f-42b5-90a0-b0ce52712f20.png)

* << operator와 >> operator을 오버로딩하여 버퍼에 읽고 쓸 수 있으며 가변 길이 데이터는 가변길이 Count를 패킷에 같이 보내어 가변길이를 Count만큼 읽어들일 수 있다.
* 채팅 서버 클라이언트의 경우 채팅 메세지 자체가 가변 길이 데이터이므로 유니코드 문자열 wchart_t 형으로 최대 채팅 입력 문자열 길이 만큼 버퍼를 할당 받은 후 채팅 메세지를 쓴다.
