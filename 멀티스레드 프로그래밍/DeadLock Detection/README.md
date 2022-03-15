## Dead-Lock Detection   
* 데드락 상황은 nullptr 체크 누락이 거의 0 순위로 제일 많은 원인이 된다.   
* 데드락은 그래프 알고리즘의 사이클 발생 여부로 판별할 수 있다.   
- 순방향 간선   
- 교차 간선   
- 역방향 간선   
* Thread 1 -> 2 -> 3 -> 1 순서로 Lock을 걸어 고의로 DeadLock 상황을 만들었을 경우
![image](https://user-images.githubusercontent.com/68372094/158335776-c3dfbf8f-52af-4372-a161-c21bf1e111ab.png)
![image](https://user-images.githubusercontent.com/68372094/158335979-60ecdfa7-2a7b-49ed-b682-eb67ca892a33.png)
![image](https://user-images.githubusercontent.com/68372094/158336011-c192aad3-6edf-4700-93e5-811788bc5a39.png)
