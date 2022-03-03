#include "pch.h"
#include "Session.h"
#include "SocketUtils.h"
#include "Service.h"

/*--------------
	Session
---------------*/

Session::Session()
{
	_socket = SocketUtils::CreateSocket();
}

Session::~Session()
{
	SocketUtils::Close(_socket);
}

void Session::Disconnect(const WCHAR* cause)
{
	//기존의 값이 이미 false이면 false를 뱉는다.
	if (_connected.exchange(false) == false)
		return;

	// TEMP
	wcout << "Disconnect : " << cause << endl;

	OnDisconnected(); // 컨텐츠 코드에서 오버로딩
	SocketUtils::Close(_socket);
	GetService()->ReleaseSession(GetSessionRef());
}

HANDLE Session::GetHandle()
{
	return reinterpret_cast<HANDLE>(_socket);
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	//이벤트 타입을 체크한다.
	switch (iocpEvent->eventType)
	{
	case EventType::Connect:
		ProcessConnect();
		break;
	case EventType::Recv:
		ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}

void Session::RegisterConnect()
{
}

void Session::RegisterRecv()
{
	//연결이 끊기면 리턴
	if (IsConnected() == false)
		return;

	//아래와 같이 생성하여 recvEvent를 넘겨줘도 된다.
	//만들고 삭제하고 반복한다.
	//세션마다 하나씩 갖고 있어야하므로 멤버변수로 해도 괜찮을거 같다.
	//RecvEvent *recvEvent = xnew<RecvEvent>();
	//recvEvent->owner = shared_from_this();
	_recvEvent.Init();
	//WSARecv 해놓는 동안 session이 삭제되지 않기 위해
	_recvEvent.owner = shared_from_this(); // ADD_REF 레퍼런스 카운트 1늘린다.

	WSABUF wsaBuf;
	wsaBuf.buf = reinterpret_cast<char*>(_recvBuffer);
	wsaBuf.len = len32(_recvBuffer);

	DWORD numOfBytes = 0;
	DWORD flags = 0;
	//																				RecvEvent(Overlapped 상속)
	if (SOCKET_ERROR == ::WSARecv(_socket, &wsaBuf, 1, OUT &numOfBytes, OUT &flags, &_recvEvent, nullptr))
	{
		int32 errorCode = ::WSAGetLastError();
		//pending이 아니면 진짜 에러다. iocp 완료통지가 뜨지 않아 영영 ref 카운트가 줄지 않기 때문에 nullptr 처리 해줘야한다.
		//pending  상태면 언젠자는 WSARecv에 대한 완료 통지가 온다.
		if (errorCode != WSA_IO_PENDING)
		{
			HandleError(errorCode);
			_recvEvent.owner = nullptr; // RELEASE_REF
		}
	}
}

void Session::RegisterSend()
{
}

void Session::ProcessConnect()
{
	_connected.store(true);

	// 세션 등록, 서비스에 세션 추가.
	GetService()->AddSession(GetSessionRef());

	// 컨텐츠 코드에서 오버라이딩
	OnConnected();

	// 수신 등록 recv Iocp 걸어준다.
	RegisterRecv();
}

void Session::ProcessRecv(int32 numOfBytes)
{
	//nullptr로 밀어줘야 RefCount 가 0 이되어 소멸자가 호출된다.
	_recvEvent.owner = nullptr; // RELEASE_REF

	//recv가 0바이트면 연결이 끊긴 상태
	if (numOfBytes == 0)
	{
		Disconnect(L"Recv 0");
		return;
	}

	// TODO
	cout << "Recv Data Len = " << numOfBytes << endl;

	// 수신 등록
	RegisterRecv();
}

void Session::ProcessSend(int32 numOfBytes)
{
}

void Session::HandleError(int32 errorCode)
{
	switch (errorCode)
	{
	case WSAECONNRESET:
	case WSAECONNABORTED:
		Disconnect(L"HandleError");
		break;
	default:
		// TODO : Log
		//콘솔로그도 컨텍스트 스위칭이 있기 때문에 스레드를 따로 만들어 찍기도 한다.
		cout << "Handle Error : " << errorCode << endl;
		break;
	}
}
