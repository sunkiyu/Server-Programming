#pragma once

/*----------------
	IocpObject
-----------------*/
//IocpObject 가 살아있는지 확인 하기 위해 레퍼런스 카운팅을 사용할 수 있다.
//외부에서 shared_ptr등을 사용하면 레퍼런스 카운팅을 이중으로 관리하여 문제가 생길 수 있다.
//enable_shared_from_this를 상속하여 자기 자신을 참조한다.
class IocpObject : public enable_shared_from_this<IocpObject>
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) abstract;
};

/*--------------
	IocpCore
---------------*/

class IocpCore
{
public:
	IocpCore();
	~IocpCore();

	HANDLE		GetHandle() { return _iocpHandle; }

	bool		Register(IocpObjectRef iocpObject);
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle;
};
