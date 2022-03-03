#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

//전방선언
class Service;

/*--------------
	Session
---------------*/

class Session : public IocpObject
{
	//다른 클래스에서 세션에 접근 가능하도록 friend로 한다.
	friend class Listener;
	friend class IocpCore;
	friend class Service;

public:
	Session();
	virtual ~Session();

public:
	//해킹 의심이나 상대방 연결이 끊겼을 경우 강제로 Disconnect 호출
	void				Disconnect(const WCHAR* cause);
	//서비스를 받는다.
	shared_ptr<Service>	GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
						/* 정보 관련 */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	//연결 되었나?
	bool				IsConnected() { return _connected; }
	//Shared 포인터로 자기 자신을 받는다.
	//자기 자신에 대한 shared_ptr을 뱉어준다.
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
						/* 인터페이스 구현 */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
						/* 전송 관련 */
						//클라이언트 단에서 IOCP를 걸어줄때 사용
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend();

	void				ProcessConnect();
	//전송 받은 데이터 수
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);
	//에러코드
	void				HandleError(int32 errorCode);

protected:
						/* 컨텐츠 코드에서 오버라이딩 하여 사용해라 */
	virtual void		OnConnected() { }
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	// TEMP
	char _recvBuffer[1000];

private:
	//서비스에 자신을 등록하거나 끊기위해 서비스 등록
	weak_ptr<Service>	_service;
	SOCKET				_socket = INVALID_SOCKET;
	NetAddress			_netAddress = {};
	Atomic<bool>		_connected = false;

private:
	USE_LOCK;

	/* 수신 관련 */

	/* 송신 관련 */

private:
						/* IocpEvent 재사용 */
	RecvEvent			_recvEvent;
};

