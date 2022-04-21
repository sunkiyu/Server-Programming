# 패킷 직렬화 \#3
![image](https://user-images.githubusercontent.com/68372094/163942941-469a05d2-839b-4fa1-9b7e-c7c0c536d161.png)
* BufferWriter가 가르키고 있는 ChunkBuffer에 구조체 포인터로 접근하여 구조체에 형식에 맞게 데이터를 쓰며 Offset으로 위치를 파악한다.
* 채팅 서버의 경우 클라이언트로 부터 받은 직렬화된 메세지를 wchar_t* 로 접근하여 유니코드 문자열을 읽는다. PKT_S_CHAT(헤더)+문자열 구조(wchar_t*)
***
![image](https://user-images.githubusercontent.com/68372094/163943460-59bc42e0-59db-4c09-a5c8-ab8b17bf04d0.png)
* Client에서도 마찬가지로 직렬화된 데이터를 구조체 포인터로 가르키게 되면 해당 구조체에 맵핑된 데이터를 볼 수 있다.
* 채팅 클라이언트의 경우 PKT_S_CHAT(헤더)와 콘솔화면에서 입력받은 유니코드 문자열을 ChunkBuffer에 wchar_t* 형으로 문자열을 쓴다. PKT_S_CHAT(헤더)+문자열 구조(wchar_t*)
