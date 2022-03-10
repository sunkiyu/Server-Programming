## Future
* 비동기적 실행
> 동기 - 함수 호출 후 I/O 작업 및 로직 처리를 마친 시점과 함수의 리턴 시점이 같다.   
비동기 - 함수 호출 후 바로 리턴한다(관심있는 이벤트를 운영체제에 맡기거나 등록해놓는다),   백그라운드에서 I/O작업 및 로직처리를 한 뒤 완료되면 프로그램에 알린다.
* deferred ->lazy evaluation 지연해서 실행
* async ->별도의 쓰레드를 만들어서 실행
```cpp
std::future<int64> future = std::async(std::launch::async, Process);
	
future.wait(); // future.wait_for(INFINITE);
std::future_status status = future.wait_for(1ms);
if (status == future_status::ready)
{

}
int64 sum = future.get(); 
```
