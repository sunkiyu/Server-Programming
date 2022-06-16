# Password Encryption

## DataBase에 패스워드를 암호화하여 저장한다.
### 암호화 라이브러리 OpenSSL 1.1.1o
### 암호화 알고리즘 AES 128 비트 seed_cbc 암호화 블록 사용, 대칭키 암호화    
### 16바이트 대칭키와 초기화 벡터값 16바이트가 필요하다.
### 128바이트 블록 단위로 암호화를 진행하므로 맨 처음에는 암호화된 값이 없으므로 초기화 벡터를 사용한다.  
### 암호 응용에서 다루는 데이터의 비트 길이가 항상 128의 배수가 된다는 보장이 없기 때문에,
### CBC 모드를 사용할 경우 입력 데이터의 비트 길이가 항상 128의 배수가 되도록 보정하는 방법이 별도로 제공된다. 일명 패딩 기법.  
### LEA(Lightweight Encryption Algorithm)를 사용한다.

![image](https://user-images.githubusercontent.com/68372094/174010979-98417b0c-5305-44b1-9058-4d07a2da413a.png)    
* 패스워드를 pwdTest1 으로 설정하였다.

![image](https://user-images.githubusercontent.com/68372094/174011088-6c935dfe-ca19-4d66-876c-048440d5c491.png)      
* 회원가입 완료

![image](https://user-images.githubusercontent.com/68372094/174011224-2968dbf4-b3ce-4b51-9c58-43e440592378.png)
* DB에 평문 pwdTest1이 암호화되어 저장되었다. 암호화된 데이터의 binary 값이다.

![image](https://user-images.githubusercontent.com/68372094/174011549-09a9c8d2-180e-40b2-8273-8afc7d241bbe.png)   
* 로그인 창에서 암호를 입력하여 로그인 시도한다. pwd라는 임의의 암호를 입력하였다.

![image](https://user-images.githubusercontent.com/68372094/174011486-138af293-85ba-4489-877f-a78b1e8b4062.png)   
![image](https://user-images.githubusercontent.com/68372094/174011871-d13b449c-2f46-4342-b8c9-d65727558ef1.png)   
* 복호화된 암호 데이터를 보기 위해 디버깅 값을 보았다.   
* pwdTest1으로 복호화 된것을 볼 수 있다. 이 때 복호화된 패스워드와 임의로 입력한 pwd값이 같지 않기 때문에 로그인 실패가 뜬다.   
