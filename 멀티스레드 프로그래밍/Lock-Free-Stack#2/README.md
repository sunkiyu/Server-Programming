## Lock-Free Stack2   
Node 데이터 구조를 이용한다.   
```cpp
template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value), next(nullptr)
		{

		}

		T data;
		Node* next;
	};

public:
	void Push(const T& value)
	{
		Node* node = new Node(value);
		node->next = _head;
	        //Thread1 이 node 1을 생성하고 현재 스택에는 head->2->3이 있다고 가정하자. head == node(2)인 상태
		//node->next= _head에서 node 1의 next가 2가 되었다 즉, node(1)->2->3
		// 이때 Thread2 가 노드 0을 생성하여 CAS 연산까지 마치고 head->0->2->3으로 만들었다 치자.
		// 다시 Thread1이 CAS를 진행하려고 보니 _head가 Thread2에 의해 node(0), node->next는 아까 상태인 node(2)이다.
		//따라서 CAS _head!=node->next이니 false 가 리턴하고 
		//node->next 즉, node(1)의 next가 _head(node(0)) 이된다.
		//다른 스레드 경합이 없다면 node->next == _head ==node(0)이 되어 _head = node 가 실행되고 리턴 true하여 Thread1의 push가 동작하게 된다.
		// 즉 head->1->0->2->3 인 상태가 된다.
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
		}
	}
	
	bool TryPop(T& value)
	{
		++_popCount;

		Node* oldHead = _head;

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
		}

		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}

		value = oldHead->data;
		TryDelete(oldHead);
		return true;
	}

	void TryDelete(Node* oldHead)
	{
		if (_popCount == 1)
		{
			Node* node = _pendingList.exchange(nullptr);

			if (--_popCount == 0)
			{
				DeleteNodes(node);
			}
			else if (node)
			{
				ChainPendingNodeList(node);
			}

			delete oldHead;
		}
		else
		{
			ChainPendingNode(oldHead);
			--_popCount;
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;

		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{
		}
	}

	void ChainPendingNodeList(Node* node)
	{
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}

private:
	atomic<Node*> _head;

	atomic<uint32> _popCount = 0;
	atomic<Node*> _pendingList; 
};
```
