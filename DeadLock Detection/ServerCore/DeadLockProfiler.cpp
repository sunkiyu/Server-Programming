#include "pch.h"
#include "DeadLockProfiler.h"

/*--------------------
	DeadLockProfiler
---------------------*/

void DeadLockProfiler::PushLock(const char* name)
{
	//멀티스레드 환경에서 돌아가야하니 LockGuard를 잡는다.
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end())
	{
		//아이디가 없으면 아이디를 발급해준다.
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		//있으면 락아이디 추출
		lockId = findIt->second;
	}

	// 잡고 있는 락이 있었다면
	if (_lockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부 다시 확인한다.
		//기존에 맨 마지막으로 잡고있었던 락 정보 추출
		const int32 prevId = _lockStack.top();
		//지금 아이디와 다르다?
		if (lockId != prevId)
		{
			//사이클 확인
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				//락에 대한 정보가 없다? 그럼 히스토리에 넣어준다.
				//그래프 구조를 계속 그려주는 과정
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	//멀티스레드 환경이니 락가드를 잡아준다.
	LockGuard guard(_lock);

	//락 스택이 없으면 에러
	//버그 예방차원
	if (_lockStack.empty())
		CRASH("MULTIPLE_UNLOCK");

	//락을 꺼냈는데 이름이 일치하지 않는 경우 push/pop 순서가 무언가 꼬여있다.
	//버그 예방차원 
	int32 lockId = _nameToId[name];
	if (_lockStack.top() != lockId)
		CRASH("INVALID_UNLOCK");

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	//해당 dfs가 완벽하게 끝났느냐?
	_finished = vector<bool>(lockCount, false);
	//경로 추적
	_parent = vector<int32>(lockCount, -1);

	//lockCount 만큼 DFS를 돌린다.
	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// 연산이 끝났으면 정리한다. 리소스 해제
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	//이미 방문한 경우 리턴
	if (_discoveredOrder[here] != -1)
		return;

	//방문하고 있는 정점의 순번을 넣어준다.
	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회한다.
	auto findIt = _lockHistory.find(here);
	if (findIt == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}

	set<int32>& nextSet = findIt->second;
	//here 정점과 인접한 정점들의 모임
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there보다 먼저 발견되었다면, there는 here의 후손이다. (순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 순방향이 아니고, Dfs(there)가 아직 종료하지 않았다면, there는 here의 선조이다. (역방향 간선)
		//사이클이 있는 상황이므로 Crash를 낸다.
		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _idToName[here], _idToName[there]);

			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}

			CRASH("DEADLOCK_DETECTED");
		}
	}

	_finished[here] = true;
}
