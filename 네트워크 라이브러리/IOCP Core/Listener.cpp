#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"

/*--------------
	Listener
---------------*/

Listener::~Listener()
{
	SocketUtils::Close(_socket);

	for (AcceptEvent* acceptEvent : _acceptEvents)
	{
		// TODO

		xdelete(acceptEvent);
	}
}

bool Listener::StartAccept(NetAddress netAddress)
{
	//소켓 생성
	_socket = SocketUtils::CreateSocket();
	if (_socket == INVALID_SOCKET)
		return false;

	//리슨 소켓도 관찰 대상이므로 등록한다.
	if (GIocpCore.Register(this) == false)
		return false;

	//주소 재사용. 
	if (SocketUtils::SetReuseAddress(_socket, true) == false)
		return false;

	if (SocketUtils::SetLinger(_socket, 0, 0) == false)
		return false;

	//bind 해준다.
	if (SocketUtils::Bind(_socket, netAddress) == false)
		return false;

	//리슨해준다.
	if (SocketUtils::Listen(_socket) == false)
		return false;

	//여기서 accept 해준다.
	const int32 acceptCount = 1;
	for (int32 i = 0; i < acceptCount; i++)
	{
		AcceptEvent* acceptEvent = xnew<AcceptEvent>();
		_acceptEvents.push_back(acceptEvent);
		//accept 해준다.
		RegisterAccept(acceptEvent);
	}

	return false;
}

void Listener::CloseSocket()
{
	SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	ASSERT_CRASH(iocpEvent->GetType() == EventType::Accept);
	AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(iocpEvent);
	ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
	//클라의 정보를 받는다.
	Session* session = xnew<Session>();

	acceptEvent->Init();
	//acceptEvent에 session을 연동시켜줘야 Dispatch 해서 해당 이벤트를 뽑아올떄 어떤 세션과 연결되었는지 알 수 있다.
	acceptEvent->SetSession(session);

	DWORD bytesReceived = 0;
	//함수 특성상 client 소켓을 미리 받는다.
	//client와 관련된 모든 정보를 session 클래스에 넣어준다.
	//비동기 accept
	//																								MSDN 참고																사실상 overlapped 구조체의 주소
	if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer, 0, sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16, OUT & bytesReceived, static_cast<LPOVERLAPPED>(acceptEvent)))
	{
		const int32 errorCode = ::WSAGetLastError();
		if (errorCode != WSA_IO_PENDING)
		{
			//accept가 풀렸으니
			// 일단 다시 Accept 걸어준다
			RegisterAccept(acceptEvent);
		}
	}
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
	Session* session = acceptEvent->GetSession();

	if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
	{
		//문제가 일어나도 accept 하기 위해 다시 register 해준다.
		RegisterAccept(acceptEvent);
		return;
	}

	SOCKADDR_IN sockAddress;
	int32 sizeOfSockAddr = sizeof(sockAddress);
	if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress), &sizeOfSockAddr))
	{
		RegisterAccept(acceptEvent);
		return;
	}

	//클라이언트 정보 저장

	session->SetNetAddress(NetAddress(sockAddress));

	cout << "Client Connected!" << endl;

	// TODO

	RegisterAccept(acceptEvent);
}
