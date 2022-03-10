#pragma once

#include <mutex>
#include <atomic>

template<typename T>
class LockStack
{
public:
	LockStack() {}
	LockStack(const LockStack&) = delete;
	LockStack& operator =(const LockStack&) = delete;

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

	bool Empty()
	{
		lock_guard<mutex> lock(_mutex);
		return _stack.empty();
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
		 
private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) :data(value)
		{

		}

		T data;
		Node *next;
	};

public:
	void Push(const T& value)
	{
		Node *node = new Node(value);
		//_head 여러 쓰레드가 접근할 수 있으므로 Data Race가 일어난다.
		node->next = _head;

		////compare_exchange 의사코드(CAS)
		//if (_head == node->next) {
		//	_head = node;//expected와 같다면 desired로 바꿔라
		//	return true;
		//}
		//else
		//{
		//	node->next = _head;
		//	return false;
		//}
		//node->next가 head와 같다면 node를 넣어라
		while (_head.compare_exchange_weak(node->next,node)==false)
		{
			//node->next = _head
		}
		//_head = node;
	}
	bool TryPop(T &value)
	{
		Node *oldHead = _head;
		
		/*if (_head == oldHead)
		{
			_head = oldHead->next;
			return true;
		}
		else
		{
			oldHead = _head;
			retrun false;
		}*/
	
		//다른 스레드가 oldHead를 삭제한 경우 crash가 날 수 있다. 
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next)==false)
		{

		}

		if (oldHead == nullptr)
			return false;
		//exception X
		value = oldHead->data;

		//잠시 삭제 보류
		//delete oldHead;
		//C# , Java와 같은 GC가 있으면 끝
		return true;
	}

private:
	atomic<Node*> _head;
};
