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
		//Pop 시도 할 경우 popCount 1 증가
		++_popCount;

		//현재 Stack의 Top 노드를 oldHead로 가르킨다.
		//_head가 클랙스 내 전역 변수이므로 경합이 발생할 수 있다.
		Node* oldHead = _head;

		//_head == oldHead 일 경우 _head = oldHead->next하고 true를 리턴해라
		//_head != oldHead 일 경우 _oldHead = _head 하고 false를 리턴해라
		//다른 스레드가 경합에 의해 이 부분에서 기존 TopNode를 Pop 하였다면 false를 리턴한다.
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
		     //oldHead = _head
		}
		
		//oldHead==nullptr 즉, 스택에 아무값도 없다면 return false;
		if (oldHead == nullptr)
		{
			--_popCount;
			return false;
		}

                //Top Node의 데이터를 value로 넘긴다.
		value = oldHead->data;
		
		//oldHead 즉 Top 노드를 지울수 있을까?
		//바로 지워버린다면 뒤늦게 들어온 Thread의 oldHead가 Dangling Pointer가 될 수 있다.
		TryDelete(oldHead);
		return true;
	}

	void TryDelete(Node* oldHead)
	{
		//Pop을 시도한 Thread가 자기 자신밖에 없을 경우
		if (_popCount == 1)
		{
			//삭제 예약된 Top노드를 가져온다.
			Node* node = _pendingList.exchange(nullptr);

			//popCount를 1 줄인 값이 0이라면 node를 삭제한다.
			if (--_popCount == 0)
			{
				DeleteNodes(node);
			}
			else if (node)
			{
				//누군가 끼어들었다면 PendingList에 추가
				ChainPendingNodeList(node);
			}

			//현재 Top노드는 삭제
			delete oldHead;
		}
		else
		{
			//popCount !=1 로 누군가 끼어든 상황이라면 pendingList에 추가만 한다.
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
			
	        //기존 PendingList를 뒤에 붙인다.
		ChainPendingNodeList(node, last);
	}

	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}
	
	//PendingList에 포함된 모든 노드를 삭제한다.
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
