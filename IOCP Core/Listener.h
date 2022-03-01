#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class AcceptEvent;

/*--------------
	Listener
---------------*/
//IocpObject로 처리하여 IocpCore에 등록해줄 것이다.
//관심대상으로 살펴봐라
class Listener : public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/* 외부에서 사용 */
	bool StartAccept(NetAddress netAddress);
	void CloseSocket();

public:
	//iocpObject의 virtual 함수 구현
	/* 인터페이스 구현 */
	virtual HANDLE GetHandle() override;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* 수신 관련 */
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

protected:
	SOCKET _socket = INVALID_SOCKET;
	Vector<AcceptEvent*> _acceptEvents;
};

