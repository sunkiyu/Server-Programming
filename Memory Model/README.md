## 메모리 모델
* 여러 쓰레드가 동일 메모리에 동시 접근 그 중 write가 문제가 된다
* atomic is_lock_free 원자적으로 처리되었는가?
```cpp
atomic<bool> flag;
//true =>원래 원자적으로 처리되었다.(된다) 
//false 원자적 처리가 필요하다.
flag.is_lock_free();

//flag 값을 prev에 넣기 전에 다른 쓰레드가 수정할 수 있다.
//이전 값을 넣고 true로 바꾸겠다. 아토믹한 연산
bool prev = flag.exchange(true);
	
//CAS(compare and swap) 조건부 수정
{
	//expected 예상한값. flag와 expected 가 같은 값인가? 그럼 desired로 바꾼다.
	bool expected = false;
	bool desired = true;
	flag.compare_exchange_strong(expected, desired);
}
```
### Memory model 정책

#### 1.Sequentially Consistent(seq_cst)
#### 2.Acquire-release(consume,acquire,release,acq_rel)
#### 3.Relaxed(relaxed)

* __seq_cst__(가장 엄격)   
-컴파일러 최적화 여지 적음 직관적, atomic 동작원리   
-가시성 코드재배치 바로 해결
	
* __acquire_release__ 핵심   
-seq_cst보다 덜 엄격하다   
-release 명령 이전의 메모리 명령들이 해당 명령 이후로 재배치 되는 것을 금지   
-acquire 로 같은 변수를 읽는 스레드가 있다면 release 이전의 명령들이 acquire하는 순간에 관찰 가능 가시성 보장   

* __consume__    
-사용 빈도 낮다.   

* __relaxed__   
-자유롭다      
-컴파일러 최적화 여지 많음 직관적이지 않음      
-코드재배치 가시성 해결 못한다.   
-가장 기본 조건 동일 객체에 대한 동일 관전 순서만 보장   

* Intel, AMD의 경우 순차적 일관성을 보장하므로 seq_cst를 써도 별다른 부하 없음   
* ARM의 경우 차이가 있다.   
