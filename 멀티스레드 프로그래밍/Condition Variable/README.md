## Condition Variable   
* Event는 커널 영역에서 실행 Condition Variable은 유저 영역에서 실행된다.
* condition_variable 클래스는 멀티쓰레드 환경에서 대기와 통지를 통해 공유 변수를 안전하게 수정할 수 있다.   
* 보통 뮤텍스 함께 사용하여 Thread Safe하다.
* condition_variable 클래스는 다른 스레드가 공유 변수(조건)를 수정하고, condition_variable을 알릴 때까지    
  하나의 스레드 또는 동시에 여러 스레드를 차단하는 데 사용할 수 있는 동기화 기본 요소.
-공유 변수를 수정하려는 스레드는 다음을 수행해야 한다.
	1. std::mutex를 획득해야 한다. (일반적으로 lock_guard를 통해 획득한다.)
	2. 락이 걸린 상태에서 공유 변수를 수정한다.
	3. std::condition_variable에 대해 notify_one 또는 notify_all을 통해 깨운다.(락을 푼다.).

* 공유 변수가 atomic 한 경우에도 대기 스레드에 수정 사항을 올바르게 전달하려면 뮤텍스 락을 건 후 수정해야 함.
- std::condition_variable에서 대기하는 모든 스레드는 다음을 수행.
	1. 공유 변수를 보호하는 데 사용된 동일한 뮤텍스에서 std::unique_lock<std::mutex> 획득
	2. wait, wait_for 또는 wait_until을 실행. 대기 작업은 atomic하게 뮤텍스를 해제하고 스레드 실행을 일시 중단.
	3. 조건 변수가 알림을 받거나 시간 초과가 만료되거나 조건 충족이 실패하면 스레드가 깨어나고 뮤텍스가 원자적으로 다시 획득됩니다. 
	  그런 다음 스레드는 조건을 확인하고 조건 충족이 실패한 경우 대기를 재개.
