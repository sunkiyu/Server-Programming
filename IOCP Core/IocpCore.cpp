#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

// TEMP
IocpCore GIocpCore;

/*--------------
	IocpCore
---------------*/
//컴플리션 포트를 만들어 주고 -> 등록 ->Dispatch 하여 워커 스레드를 통해 일감 처리
IocpCore::IocpCore()
{
	_iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
	::CloseHandle(_iocpHandle);
}
//listen 소켓과 관련된 것을 등록해서 처리 가능
bool IocpCore::Register(IocpObject* iocpObject)
{
	//원래는 Socket을 받았다.
	//stl queue를 사용하듯 다양한 것을 인자로 넘길 수 있다.
	//일반 적으로 소켓을 사용한것과 같은 너낌.
	//관찰 대상에 iocpObject를 넣는다.
	//나중에 레퍼런스 카운팅 관리도 해줘야 한다.
	return ::CreateIoCompletionPort(iocpObject->GetHandle(), _iocpHandle, reinterpret_cast<ULONG_PTR>(iocpObject), 0);
}

bool IocpCore::Dispatch(uint32 timeoutMs)
{
	//일감이 있는지 계속 체크한다.]
	//송/수신된 바이트
	DWORD numOfBytes = 0;
	//iocp 키 값 역할, 이전 프로젝트 session 역할
	IocpObject* iocpObject = nullptr;
	//overlapped 구조체 역할
	IocpEvent* iocpEvent = nullptr;

	if (::GetQueuedCompletionStatus(_iocpHandle, OUT &numOfBytes, OUT reinterpret_cast<PULONG_PTR>(&iocpObject), OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		//여기까지 왔으면 정상적으로 iocpObject iocpEvent 복원 되었다.
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			// TODO : 로그 찍기
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}
