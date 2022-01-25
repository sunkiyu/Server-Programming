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
		Node(const T& value) :data(value),next(nullptr)
		{

		}

		T data;
		Node *next;
	};

public:
	//Push는 나만 사용하니 문제가 안된다. 스레드 별로 독립적으로 쓴다.
	void Push(const T& value)
	{
		Node *node = new Node(value);
		//_head 여러 쓰레드가 접근할 수 있으므로 Data Race가 일어난다.
		node->next = _head;


		//node->next가 head와 같다면 node를 넣어라
		while (_head.compare_exchange_weak(node->next,node)==false)
		{
		
		}
	
	}
	//스택에서 뺄 때 여러 스레드가 접근 할 경우 Data Race가 날 수 있다.
	bool TryPop(T &value)
	{
		++_popCount;
		Node *oldHead = _head;
		

		//다른 스레드가 oldHead를 삭제한 경우 crash가 날 수 있다. 
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next)==false)
		{

		}

		if (oldHead == nullptr) {
			--_popCount;
			return false;
		}
		//exception X
		value = oldHead->data;

		TryDelete(oldHead);
		//잠시 삭제 보류

		return true;
	}

	void TryDelete(Node * oldhead)
	{
		//나 외에 누가 있는가?
		//나만 존재
		if (_popCount == 1)
		{
			//삭제 예약된 다른 데이터들도 삭제시도
			//데이터를 분리 아토믹하게 실행됨
			Node *node = _pendingList.exchange(nullptr);
			if (--_popCount == 0)
			{
				//끼어든 스레드가 없다. 삭제 진행
				DeleteNodes(node);
			}
			else if(node)
			{
				//끼어든 스레드가 있다. 다시 갖다 놓는다.
				ChainPendingNodeList(node);
			}

			delete oldhead;
		}
		//다른 스레드도 pop하고 있는 경우
		else
		{
			//당장 삭제하지 않고 삭제 예약
			ChainPendingNode(oldhead);
			--_popCount;
		}
	}
	
	void ChainPendingNodeList(Node *first, Node *last)
	{
		last->next = _pendingList;
		while (_pendingList.compare_exchange_weak(last->next,first)==false)
		{

		}
	}

	void ChainPendingNodeList(Node *node)
	{
		Node *last = node;
		while (last->next)
			last = last->next;
		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node *node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node * node)
	{
		while (node)
		{
			Node *next = node->next;
			delete node;
			node = next;
		}
	}

private:
	atomic<Node*> _head;
	atomic<uint32> _popCount = 0;	//pop을 실행중이 스레드 개수
	atomic<Node*> _pendingList;		//삭제 되어야 할 노드(첫번째 노드)
};
