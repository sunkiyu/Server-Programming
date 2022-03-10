## Lock-Free Stack2   
Node 데이터 구조를 이용한다.   
```cpp
struct Node
{
	Node(const T& value) :data(value),next(nullptr)
	{

	}

	T data;
	Node *next;
};
```
