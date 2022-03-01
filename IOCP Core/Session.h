#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/*--------------
	Session
---------------*/
//Listener와 마찬가지로 iocpCore에 등록할 대상
class Session : public IocpObject
{
public:
	Session();
	virtual ~Session();

public:
	/* 정보 관련 */
	void		SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress	GetAddress() { return _netAddress; }
	SOCKET		GetSocket() { return _socket; }

public:
	/* 인터페이스 구현 */
		//icopobject virtual 함수 구현
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

public:
	// TEMP
	char _recvBuffer[1000];

private:
	SOCKET			_socket = INVALID_SOCKET;
	NetAddress		_netAddress = {};
	Atomic<bool>	_connected = false;
};

