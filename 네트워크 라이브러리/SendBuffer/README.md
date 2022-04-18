# Send Buffer

![image](https://user-images.githubusercontent.com/68372094/162970452-f50116da-47f7-4822-b37e-640261815b6b.png)

* 브로드캐스팅 관련
* Send는 멀티스레드 환경에서 Thread Safe 하기위해 별도의 작업이 필요하다.
* SendBuffer를 생성하여에 데이터를 쓴 후 SendQueue에 포인터를 넘긴다. 
* SendQueue데이터를 포인터로 참조한 뒤 Pop 한다. -> Pop 하는 과정에서 댕글링 포인터를 방지하기 위해 RefCount를 1 올려놓기 위함이다.
* Pop한 데이터를 WSABUF 구조체 벡터에 모은 뒤 한번에 WSASend 한다.
* 이 때 Pop & Send 작업은 한 스레드가 할 수 있도록 Lock을 잡는다.
* 따라서 한 스레드가 Pop & Send 작업동안 다른 스레드는 접근하지 못하고 SendQueue에 데이터를 밀어넣기만 한다.
