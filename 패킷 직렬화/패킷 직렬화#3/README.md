# 패킷 직렬화 \#3
![image](https://user-images.githubusercontent.com/68372094/160074018-efba56c3-dde0-4bd4-92ef-db55534e3c45.png)
* BufferWriter가 가르키고 있는 ChunkBuffer에 구조체 포인터로 접근하여 구조체에 형식에 맞게 데이터를 쓰며 Offset으로 위치를 파악한다.
***
![image](https://user-images.githubusercontent.com/68372094/160074326-bab80d02-e56b-4316-a5ac-b91695893f57.png)
* Client에서도 마찬가지로 직렬화된 데이터를 구조체 포인터로 가르키게 되면 해당 구조체에 맵핑된 데이터를 볼 수 있다.
