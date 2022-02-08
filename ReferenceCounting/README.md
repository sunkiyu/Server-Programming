
## 레퍼런스-카운팅   
* unique_ptr은 C스타일 포인터와 거의 같다. 차이점은 복사하는 부분(복사 생성자)이 막혀있다. std::move를 통해서(이동 대입 연산자) 대입 연산 가능   
* weak_ptr은 shared_ptr의 순환 참조 문제 해결.   
* shared_ptr 사용시 reference Count도 같이 관리된다.   
* weak_ptr은 reference Count만 체크한다. 객체의 수명 주기에는 영향을 미치지 않는다.   
