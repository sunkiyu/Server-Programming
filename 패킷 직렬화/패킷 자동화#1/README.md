# 패킷 자동화\#1

* 패킷 핸들러 함수를 담고 있을 배열을 선언하고 핸들러 함수 포인터를 넣어준다.     
* 배열의 인덱스는 처리할 패킷 ID 이고 해당 인덱스에 들어있는 함수 포인터는 해당 패킷 ID를 처리할 핸들러 함수이다.   
* 패킷을 받으면 OnRecvPacket 함수에서 HandlePacket 함수를 호출한다.    
* HandlePacket 함수에서는 패킷 ID를 추출하여 패킷 핸들러 함수 배열의 패킷 ID 인덱스에 해당하는 핸들러 함수를 호출한다.   

```cpp
//패킷 핸들러 함수를 담고 있는 배열
//PKT_S_TEST가 패킷 ID이다.
SKPacketHandler[PKT_S_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) {
  return HandlePacket<Protocol::S_TEST>(Handle_S_TEST, session, buffer, len); 
};

//패킷 핸들러 함수는 템플릿으로 선언하여 여러가지 패킷 ID를 처리할 수 있도록 한다.
template<typename PacketType, typename ProcessFunc>
static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
{
     //func : 핸들러 함수 포인터
     //PacketType : S_TEST
     //session, buffer, len 차례로 넣는다.
     ...
}
```
* 패킷 핸들러 함수를 일일이 만들어주는 것은 비효율적이다. 따라서 자동화가 필요하다.     
![image](https://user-images.githubusercontent.com/68372094/160997708-ab8e0ec5-1453-47f5-9b39-01c18f1dc623.png)
***
![image](https://user-images.githubusercontent.com/68372094/160998639-10cdbefd-fb39-4914-8f8b-11911718b45d.png)

