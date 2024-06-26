# ProtoBuf(Protocol Buffer)

## 사용법

### Step1
https://github.com/protocolbuffers/protobuf/releases 에서 Google Protobuf 컴파일러를 다운받는다.
![image](https://user-images.githubusercontent.com/68372094/160231769-dd836832-2e04-4834-9e64-716d29ac6043.png)
* 컴파일러는 프로토파일을 작성하면 언어별로 컴파일하여 소스코드를 자동 생성해준다.

### Step2
* https://developers.google.com/protocol-buffers/docs/proto3 문서를 보며 프로토파일을 작성한다.

### Step3
![image](https://user-images.githubusercontent.com/68372094/160231844-8802f4a5-dec8-466b-8439-edd030b177b5.png)
* 컴파일 환경을 설정 및 컴파일

### Step4
* 생성된 .cc 파일과 .h 파일을 프로젝트 폴더로 가져온다.

### Step5
* Google ProtoBuf 라이브러리를 다운받는다.
* https://github.com/protocolbuffers/protobuf/branches

### Step6
* Cmake를 다운받는다.
* Step5에서 다운받은 Cmake 디렉터리 경로를 입력해준다.
* Debug/Release 용 라이브러리가 생성된다.

### Step6
* 프로젝트에 라이브러리를 링크시켜 사용한다.

![image](https://user-images.githubusercontent.com/68372094/160517589-ac891234-f96a-438b-be93-4efa3810d528.png)  
* 서버 측에서 ProtoBuf의 SerializeToArray 함수로 패킷을 직렬화하여 클라이언트에게 전송한다.
***
![image](https://user-images.githubusercontent.com/68372094/160519706-4aafcd8c-b736-4568-ad14-311e34713c56.png)
* 클라이언트가 데이터를 받게되면 OnRecvPacket에서 패킷 헤더의 ID를 검사한 뒤 해당 헤더의 패킷을 처리할 Handler 함수를 호출한다.    
* ProtoBuf에서 제공하는 ParseFromArray로 데이터를 파싱하여 가져온다.   

