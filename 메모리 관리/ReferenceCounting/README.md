
## 레퍼런스-카운팅   
* unique_ptr은 C스타일 포인터와 거의 같다. 차이점은 복사하는 부분(복사 생성자)이 막혀있다. std::move를 통해서(이동 대입 연산자) 대입 연산 가능   
* weak_ptr은 shared_ptr의 순환 참조 문제 해결.   
* shared_ptr 사용시 reference Count도 같이 관리된다.   
* weak_ptr은 reference Count만 체크한다. 객체의 수명 주기에는 영향을 미치지 않는다.   

* RefCountable 클래스와 TSharedPtr클래스를 통해 레퍼런스 카운트를 계산하여 따로 delete 해주지 않아도 자동 소멸되게 한다.
* 멀티스레드 환경에서 Data Race에 의해 ptr이 가르키는 객체 소멸시켜 Dangling Pointer가 발생하는 경우를 복사 생성자에서 ReferenceCount를 1 증가시켜 막아준다.

