# Job Queue#2
![image](https://user-images.githubusercontent.com/68372094/161680778-da570871-eca2-4cc3-9ff9-9e7549508021.png)
* Job Queue#1 에서는 Job 마다 클래스들을 직접 만들어 Job Queue에 넣어주었기 때문에 Job 별로 클래스를 계속 생성해주었다.    
* Job이 수백개~수천개가 된다면? 일일이 만들어 줄 것인가?   
* Job Queue#2 에서는 하나의 MemberJob이라는 템플릿 클래스를 만들어 클래스에 수행해야할 Job 함수와 인자를 직접 전달   
