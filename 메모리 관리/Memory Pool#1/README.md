<img width="737" alt="스크린샷 2022-02-09 오후 11 55 41" src="https://user-images.githubusercontent.com/68372094/153226911-917b0fb8-ae49-4f2c-aeed-2a420f48a2c9.png">

* 메모리 풀을 사이즈 별로 생성 후 메모리 풀 테이블에서 관리한다.   
* 사이즈 별 생성은 사용자 임의로 선택하면 되는데 32바이트 64바이트 96바이트 ... 등을 메모리 풀 사이즈를 선택한다.      
* 만약 32바이트 메모리 풀을 예로 들면 메모리 풀 테이블의 1~32번 인덱스가 모두 32바이트 메모리 풀 영역의 주소를 갖고 있는다.      
* 20바이트 객체를 생성하게 되면 메모리 테이블의 20번째 인덱스에 해당하는 메모리 풀을 할당받는데 해당 풀은 32바이트이다.   

## __Placement New__      
new의 역할은 2가지이며 이를 동시에 수행한다.   
1. 메모리 할당   
2. 생성자를 통한 초기화

Placement New를 통해 1번과 2번을 각각 분리된 작업으로 수행할 수 있다.
예)

```cpp
template<Typename T>
class PlacementNew
{
	public:
	PlacementNew() { }
	
	static void* AttachHeader(T* header, int32 size)
	{
		new(header)T(size); // placement new 이미 할당 받아놓은 header가 가르키는 메모리에 T 객체 생성자를 실행하여 초기화를 수행한다.
		return reinterpret_cast<void*>(++header);
	}
}
```
