# 패킷 직렬화 \#3
![image](https://user-images.githubusercontent.com/68372094/163942941-469a05d2-839b-4fa1-9b7e-c7c0c536d161.png)
* BufferWriter가 가르키고 있는 ChunkBuffer에 구조체 포인터로 접근하여 구조체에 형식에 맞게 데이터를 쓰며 Offset으로 위치를 파악한다.
***
![image](https://user-images.githubusercontent.com/68372094/163943250-3606767e-8fdb-4c27-aee2-77e9713bf796.png)
* Client에서도 마찬가지로 직렬화된 데이터를 구조체 포인터로 가르키게 되면 해당 구조체에 맵핑된 데이터를 볼 수 있다.
