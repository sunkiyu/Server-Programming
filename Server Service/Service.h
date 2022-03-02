#pragma once
#include "NetAddress.h"
#include "IocpCore.h"
#include "Listener.h"
#include <functional>

enum class ServiceType : uint8
{
	Server,
	Client
};

/*-------------
	Service
--------------*/
//세션 만들어주는 함수
using SessionFactory = function<SessionRef(void)>;

//클라일수도 있고 서버일 수도 있다. 다양한 정책
class Service : public enable_shared_from_this<Service>
{
public:
	//      클라인가 서버인가?   서버:자기자신 클라:서버                   세션을 만들어주는 함수 전달    최대 동접수
	Service(ServiceType type, NetAddress address, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~Service();

	//서비스를 상속받는 클래스가 무조건 구현해야함.
	virtual bool		Start() abstract;
	//세션 팩토리가 있어야 시작할 수 있다.
	bool				CanStart() { return _sessionFactory != nullptr; }

	virtual void		CloseService();
	//생성자에서도 넣어줬지만 나중에 바꿀 경우
	void				SetSessionFactory(SessionFactory func) { _sessionFactory = func; }
	// 세션 생성과 동시에 iocp코어에 넣는다.
	SessionRef			CreateSession();
	void				AddSession(SessionRef session);
	void				ReleaseSession(SessionRef session);
	//현재 세션은 몇개입니까?
	int32				GetCurrentSessionCount() { return _sessionCount; }
	int32				GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType			GetServiceType() { return _type; }
	NetAddress			GetNetAddress() { return _netAddress; }
	IocpCoreRef&		GetIocpCore() { return _iocpCore; }

protected:
	//락 사용
	USE_LOCK;
	ServiceType			_type;
	NetAddress			_netAddress = {};
	//어떤 iocp에 일감을 등록할 것인가?
	IocpCoreRef			_iocpCore;
	//세션들을 들고 있다.
	Set<SessionRef>		_sessions;
	int32				_sessionCount = 0;
	int32				_maxSessionCount = 0;
	//세션 생성함수 포인터를 받는다.
	SessionFactory		_sessionFactory;
};

/*-----------------
	ClientService
------------------*/

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ClientService() {}

	virtual bool	Start() override;
};


/*-----------------
	ServerService
------------------*/

class ServerService : public Service
{
public:
	ServerService(NetAddress targetAddress, IocpCoreRef core, SessionFactory factory, int32 maxSessionCount = 1);
	virtual ~ServerService() {}

	virtual bool	Start() override;
	virtual void	CloseService() override;

private:
	ListenerRef		_listener = nullptr;
};
