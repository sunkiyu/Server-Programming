## Event   
![image](https://user-images.githubusercontent.com/68372094/154230366-10277e0b-d1c7-435a-b6b4-89f0d936db63.png)
   
자동 Reset 이벤트
수동 Reset 이벤트
	
```cpp
void Producer()
{
	while (true)
	{
		{
			unique_lock<mutex> lock(m);
			q.push(100);
		}
		::SetEvent(handle);
		this_thread::sleep_for(100000000ms);
	}
};

void Consumer()
{
	while (true)
	{
		unique_lock<mutex> lock(m);
		::WaitForSingleObject(handle, INFINITE);
		//::ResetEvent(handle);
		//Signal 되었다가 Non-Signal 상태로 변함
		if (q.empty() == false)
		{
			int32 data = q.front();
			q.pop();
			cout << data << endl;
		}
	}
};
```
* CreateEvent,CreateProcess 등을 수행하면 커널 오브젝트가 생성된다.    
* UsageCount는 생성된 커널 오브젝트를 참조하는 횟수인데 CreateProcess일 경우 부모 프로세스, 자식 프로세스 총 2개의 UsageCount가 발생한다.   
* CloseHandle을 호출하게 되면 UsageCount가 감소하게 되는데 UsageCount가 0이 되었을때 해당 커널 오브젝트가 소멸된다.   
* Handle을 커널 오브젝트 정보를 참조하는 역할을 한다.
