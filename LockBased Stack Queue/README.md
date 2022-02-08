## Lock-Based Stack   
### * Lock을 걸지 않은 Stack과 Queue는 멀티스레드 환경에서 비어있는 값을 참조하는 Crash가 발생할 가능성이 있다.
### Push & Pop의 예   
				 
```cpp
void Push(T value)
{
	//Lock 을 걸고
	lock_guard<mutex> lock(_mutex);
	//우측값으로 변환후 넣는다.
	_stack.push(std::move(value));
	_condVar.notify_one();
}

bool TryPop(T &value)
{
	//Lock 을 걸고
	lock_guard<mutex> lock(_mutex);
	if (_stack.empty())
		return false;
	//
	value = std::move(_stack.top());
	_stack.pop();
	return true;
}

void WaitPop(T& value)
{
	unique_lock<mutex> lock(_mutex);
	//조건 만족하지  않으면 Lock 을 풀고 잠든다.
	//시그널이 오면 Lock을 잡고 실행
	_condVar.wait(lock, [this] {return _stack.empty() == false; });
	value = std::move(_stack.top());
	_stack.pop();
}
```   
## Lock-Based Queue   
### * Lock을 걸지 않은 Stack과 Queue는 멀티스레드 환경에서 비어있는 값을 참조하는 Crash가 발생할 가능성이 있다.
### Push & Pop의 예   
				 
```cpp
	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_queue.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T &value)
	{
		//Lock 을 걸고
		lock_guard<mutex> lock(_mutex);
		if (_queue.empty())
			return false;

		//
		value = std::move(_queue.front());
		_queue.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		//조건 만족하지  않으면 Lock 을 풀고 잠든다.
		//시그널이 오면 Lock을 잡고 실행
		_condVar.wait(lock, [this] {return _queue.empty() == false; });
		value = std::move(_queue.front());
		_queue.pop();
	}
```
