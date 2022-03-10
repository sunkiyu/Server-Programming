## Reader-Writer-Lock   
* 스레드와 락 관리
```cpp
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner ThreadId)
R : ReadFlag (Shared Lock Count)

4바이트(32비트) 값으로 ThreadId와 락을 공유하는 스레드 개수 정보를 나타낸다.
상위 2바이트(16비트) => ThreadId
하위 2바이트(16비트) => 락을 공유하는 스레드 개수

 enum : uint32
    {
    //Dead락을 감지하는 TimeOut 시간
        ACQUIRE_TIMEOUT_TICK = 10000,
    //락을 시도하는 횟수
        MAX_SPIN_COUNT = 5000,
    //상위 16비트를 뽑아낸다. =>스레드 아이디
        WRITE_THREAD_MASK = 0xFFFF'0000,
    //하위 16비트를 뽑아낸다. => 공유 락 횟수
        READ_COUNT_MASK = 0x0000'FFFF,
        EMPTY_FLAG = 0x0000'0000
    };
```cpp
//쓰레드 ID 관리
thread_local uint32 LThreadId = 0;
 Atomic<uint32> _lockFlag = EMPTY_FLAG;

//쓰기 락
void Lock::WriteLock()
{
	//시작 시간을 얻는다.
	const int64 beginTick = ::GetTickCount64();
    //스레드 아이디를 얻는다.(2바이트)
    //32비트를 왼쪽으로 16비트 쉬프트하게 되면 16비트가 남는데
    //그중 상위 16비트를 뽑아내어 (스레드 아이디)
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;
            //_lockFlag가 expected와 같으면(EMPTY_FLAG)
            //_lockFlag = desired해주고 return true;
            //같지 않으면 expected = lockFlag해주고 return false;
			if (_lockFlag.compare_exchange_strong(OUT expected, desired))
			{
				_writeCount++;
				return;
			}
		}

//시간 차이를 구해 Tick보다 크면 데드락으로 간주해 Crash를 낸다.
		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");
//자신의 실행 기회를 잠시 다른 스레드에게 양보한다.
		this_thread::yield();
	}
}

void Lock::ReadLock()
{
    //스레드 아이디를 뽑아낸다.
	const uint32 lockThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == lockThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	const int64 beginTick = ::GetTickCount64();
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = (_lockFlag.load() & READ_COUNT_MASK);
			if (_lockFlag.compare_exchange_strong(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTick >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::ReadUnlock()
{
	if ((_lockFlag.fetch_sub(1) & READ_COUNT_MASK) == 0)
		CRASH("MULTIPL
    
```   
