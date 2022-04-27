## Lock-Free Stack1
```cpp
template<typename T>
class LockFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value)
		{

		}

		T data;
		Node* next;
	};

public:
	void Push(const T& value)
	{
		Node* node = new Node(value);
		//새 노드가 Top에 위치하므로 새 노드의 next에 현재 Top(==_head)을 위치시켜준다.
		node->next = _head;
		
		//만약 _head와 node->next가 같다면 _head = node 시켜주고 true리턴
		//같지 않다면 즉, 경합이 발생하였다면, node->next = _head 시켜주고 false 리턴
		//_head가 전역 속성이므로 이 사이에 경합이 발생할 수 있다!
		while (_head.compare_exchange_weak(node->next, node) == false)
		{
		}
	}
	
	bool TryPop(T& value)
	{
		//현재 Top노드(_head)를 oldHead로 가르키게 한다.
		Node* oldHead = _head;

		//마찬가지로 _head가 전역 속성이니 이 사이에 경합이 발생할 수 있다.
		//Top 노드를 빼낼것이니 Top의 아래 노드를 _head가 가르키게 해줘야한다.
		//_head와 oldHead가 같다면 _head = oldHead->next를 넣어주고 return true;
		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
			//oldHead = _head;
		}

		if (oldHead == nullptr)
			return false;

		value = oldHead->data;

		return true;
	}

private:
	atomic<Node*> _head;
};
```
