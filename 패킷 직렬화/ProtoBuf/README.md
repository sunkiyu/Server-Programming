# ProtoBuf

* 사용법

### Step1
https://github.com/protocolbuffers/protobuf/releases 에서 Google Protobuf 컴파일러를 다운받는다.
* ![image](https://user-images.githubusercontent.com/68372094/160231769-dd836832-2e04-4834-9e64-716d29ac6043.png)
* 컴파일러는 프로토파일을 작성하면 언어별로 컴파일하여 소스코드를 자동 생성해준다.

### Step2
* https://developers.google.com/protocol-buffers/docs/proto3 문서를 보며 프로토파일을 작성한다.

### Step3
![image](https://user-images.githubusercontent.com/68372094/160231844-8802f4a5-dec8-466b-8439-edd030b177b5.png)
* 컴파일 환경을 설정해준다.

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
