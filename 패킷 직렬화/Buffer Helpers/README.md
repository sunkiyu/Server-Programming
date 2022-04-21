# Buffer Helpers

![image](https://user-images.githubusercontent.com/68372094/163809512-95c01c8a-6137-48fc-ac0a-31fb4f442232.png)
***
![image](https://user-images.githubusercontent.com/68372094/163809549-ec3d6d94-5963-43e6-83bd-4a06420055f5.png)

* BufferWriter은 << Operator을 오버로딩하여 SendBufferChunk에 << 피연산자 형식에 맞게 데이터를 쓸 수 있다.
* BufferReader은 >> Operator을 오버로딩하여 recvBuffer로 부터 >> 피연산자 형식에 맞게 데이터를 읽어 올 수 있다.
* BufferWriter과 BufferReader은 데이터를 버퍼에 읽고 쓰는데 도움을 준다.
* 채팅 서버 클라이언트의 경우 문자열 송신을 위해 wchar_t 형을 이용한다.
