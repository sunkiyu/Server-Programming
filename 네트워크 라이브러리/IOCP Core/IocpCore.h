#pragma once

/*----------------
	IocpObject
-----------------*/
//IOCP에 등록할 객체
class IocpObject
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

	//소켓을 생성한 뒤 IOCP 관찰 대상에 등록하기 위한 함수
	bool		Register(class IocpObject* iocpObject);
	//worker Thread 들이 일감을 찾는 함수
	bool		Dispatch(uint32 timeoutMs = INFINITE);

private:
	HANDLE		_iocpHandle;
};

// TEMP
extern IocpCore GIocpCore;
