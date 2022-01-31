#pragma once
#include <stack>
#include <map>
#include <vector>

/*--------------------
	DeadLockProfiler
---------------------*/
//락을 잡거나 해제할 때 사이클이 발생하는지 Check
class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 index);

private:
	//내부적으로 관리할 때는 이름보다 정수가 편하므로 id로 치환하는 Map Container을 만든다.
	unordered_map<const char*, int32>	_nameToId;
	unordered_map<int32, const char*>	_idToName;
	stack<int32>						_lockStack;
	//그래프에서 간선에 해당하는 정보... 0번 락이 1번락을 잡았다 -> _lockHistory[0] = {1,...}
	map<int32, set<int32>>				_lockHistory;
	//DeadLockProfiler가 멀티스레드 환경에서 안전하게 동작하게끔 Mutex하나를 만든다.
	Mutex _lock;

private:
	vector<int32>	_discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32			_discoveredCount = 0; // 노드가 발견된 순서
	vector<bool>	_finished; // Dfs(i)가 종료 되었는지 여부
	vector<int32>	_parent;
};

