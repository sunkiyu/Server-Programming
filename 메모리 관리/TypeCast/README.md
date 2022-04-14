# TypeCast

```cpp
template<typename From, typename To>
class From2To
{
private:
	using S = __int8;
	using B = __int32;

	static S Mapping(const To&) { return 0; }
	static B Mapping(...) { return 0; }
	static From RetFrom() { return 0; }

public:
	enum
	{
  //RetFrom()에서 From 타입이 리턴되어 Mapping 함수의 인자로 넘어가게 되는데 이때 FromType이 To Type으로 캐스팅 가능하다면
  //const To& 인자로 넘어가 S가 리턴되어 exist==1 을 만든다.
  //반면 From Type이 const To & 타입으로 캐스팅 불가능하다면 Mapping(...) 함수 인자로 넘어가 B가 리턴되며 exists==0을 만든다.
		exists = sizeof(Mapping(RetFrom())) == sizeof(S)
	};
};

How to Use
From2To<Parent,Child>::exists;//0
From2To<Child,Parent>::exists;//1
```
