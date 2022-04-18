# SendBuffer Pooling

![image](https://user-images.githubusercontent.com/68372094/163183881-650e14b1-a566-4185-9025-375b4a182d60.png)

* 기존 SendBuffer는 데이터를 송신할 때마다 Sendbuffer을 새로 생성하여 전송하기 때문에 비효율적이며 오버헤드가 존재하였다.
* 큰 덩어리의 SendBufferChunk를 만들고 SendBuffer 가 필요시 마다 SendBufferChunk에 데이터를 쓰고 해당 버퍼의 포인터를 넘겨 전송한다.
* SendBufferChunk의 usedSize 커서를 두어 데이터를 쓸 때마다 이동시킨 후 새로운 데이터를 입력할 때 usedSize 커서의 포인터를 넘겨 해당 위치에 쓰고 보낸다. 
* SendBufferManager를 두어 SendBufferChunk를 모두 사용했을시 소멸시키고 새로 만드는 방식을 채택하였다.
