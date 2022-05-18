# Job Queue#1
![image](https://user-images.githubusercontent.com/68372094/161478208-ddbf5720-b3fa-42ce-b2ce-856957a6b676.png)
1. 클라이언트가 서버에 비동기 ConnectEx를 수행한다.   
2. ConnectEx가 완료되면 클라이언트에서 C_LOGIN 패킷을 서버에 송신.    
3. 서버에서 패킷 ID(C_LOGIN)를 확인 후 C_LOGIN 핸들러를 호출   
4. 서버에서 클라이언트 정보들을 DB에서 Read하여 S_LOGIN 패킷을 클라이언트로 송신한다.   
5. 클라이언트 -> 서버로 C_ENTER_GAME 패킷 송신   
6. 서버에서는 클라이언트가 입장을 요청하는 것이므로 EnterJob을 JobQeueu에 등록한뒤, S_ENTER_GAME를 클라이언트로 송신한다.    
7. JobQueue에 등록된 Job은 메인스레드가 Pop & Excecute 한다.   
***

