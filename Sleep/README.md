## Sleep
* SpinLock에서 반복문을 통해 락 상태를 확인하며 락을 계속 확인하는데 다른 쓰레드가 lock을 한시간이 길어지면 CPU 점유율이 높아질 수 있다.
* 이 때 Sleep을 통해 잠시 TimeSlice를 반환하여 CPU 부하를 낮출 수 있다.
* this_thread::sleep_for(쓰레드를 잠시 Block 상태로 한다. 일정 시간동안 스케줄러에 선택받지 못함),   
	this_thread::yield(쓰레드를 Ready 상태로 스케줄러에 의해 선택받을 수 있게 한다)   
```cpp
while (_locked.compare_exchange_strong(expected, desired) == false)
{
	expected = false;
	//Sleep을 하면서 While문의 CPU 부하를 줄일 수 있다.
	//cout도 시간이 오래 걸리는 작업
	//커널 모드로 들어가는 것이 Context Switching 을 유발한다.
	//this_thread::sleep_for(std::chrono::milliseconds(100));			
	//this_thread::sleep_for(0ms);
	this_thread::yield();//현재 타임 슬라이스를 반환, 위의 sleep_for(0)과 같다.
}
```
