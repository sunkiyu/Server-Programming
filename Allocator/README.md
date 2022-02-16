## Allocator   
* new와 delete를 오버로딩하여 커스터마이즈 가능하다.   
* 클래스에 적용시키려면 클래스 내부에 new와 delete를 오버로딩한다.   

```cpp
void* operator new(size_t size)
{
	cout << "new! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete(void* ptr)
{
	cout << "delete!" << endl;
	::free(ptr);
}

void* operator new[](size_t size)
{
	cout << "new[]! " << size << endl;
	void* ptr = ::malloc(size);
	return ptr;
}

void operator delete[](void* ptr)
{
	cout << "delete![]" << endl;
	::free(ptr);
}
```   
* 타입에 상관없이 타입 크기에 맞는 메모리를 할당해주는 템플릿과 클래스를 생성하여 사용할 수 있다.   
![image](https://user-images.githubusercontent.com/68372094/154200945-422e0a90-a271-40d3-8566-18d98f51703a.png)   

* Placement new란?   
- Cpp의 new에서는 메모리 할당과 초기화 2가지 작업을 동시에 수행한다.   
- Placement new를 사용하면 메모리 할당과 초기화 작업을 각각 수행할 수 있는데, 미리 malloc 등으로 할당해 놓은 메모리 주소에 생성자를 실행하여 해당 메모리에 초기화 할 수 있다.   

