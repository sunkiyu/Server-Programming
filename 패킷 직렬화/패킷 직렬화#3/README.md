# 패킷 직렬화 \#3
![image](https://user-images.githubusercontent.com/68372094/160062090-cfe3f13d-e452-4982-a3be-b32f4a2c41c7.png)
* BufferWriter가 가르키고 있는 ChunkBuffer에 구조체 포인터로 접근하여 구조체에 형식에 맞게 데이터를 쓰며 Offset으로 위치를 파악한다.
***
![image](https://user-images.githubusercontent.com/68372094/160065527-8de939d8-7874-4f5a-8e93-cbc1a402b7cb.png)
* Client에서도 마찬가지로 직렬화된 데이터를 구조체 포인터로 가르키게 되면 해당 구조체에 맵핑된 데이터를 볼 수 있다.
