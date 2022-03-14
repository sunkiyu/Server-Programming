## Stomp-Allocator
```cpp
class Player
{
public:
	Player() {}
	virtual ~Player() {}
};

class Knight : public Player
{
public:
	Knight()
	{
		cout << "Knight()" << endl;
	}

	Knight(int32 hp) : _hp(hp)
	{
		cout << "Knight(hp)" << endl;
	}

	~Knight()
	{
		cout << "~Knight()" << endl;
	}

	int32 _hp = 100;
	int32 _mp = 10;
};

int main()
{
	Knight* knight2 = (Knight*)new Player();
	delete knight2;
  //반환된 메모리에 접근이 가능하다!
	knight2->_hp = 100;
```
![image](https://user-images.githubusercontent.com/68372094/158111922-9039209a-805f-45b7-b41a-7e6a7a10e075.png)

* 기존 new 와 delete는 상황에 따라 다르지만 delete한 메모리에 접근이 가능한 경우가 있으므로 위험하다.
* 운영체제에 delete 요청을 한다고 즉시 메모리 delete 되지 않을 수 있다. 

```cpp
void* baseAddress = ::VirtualAlloc(NULL, pageCount * PAGE_SIZE, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE);
```
* 페이지 단위로 메모리를 할당 받아 사용하면 OverFlow 발생시 바로 Crash가 발생한다.
* VirtualAlloc로 Customize한 xnew를 사용한다.
![image](https://user-images.githubusercontent.com/68372094/158111984-991c52ee-9f1e-4d7e-b150-e0863d8f672f.png)

