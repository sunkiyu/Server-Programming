# SocketUtils
## 확장 메커니즘
* 기존 accept, connect는 overlapped를 받는 인자가 없어 비동기로 동작하지 않았다.   
* IOCP 비동기 방식으로 I/O를 처리하는 함수를 제공한다.
* DisconnectEx를 사용하면 소켓을 재사용 할 수 있다.
* 확장 기능을 호출하려면 응용 프로그램에서 먼저 원하는 기능에 대한 포인터를 요청해야 한다.
* SIO_GET_EXTENSION_FUNCTION_POINTER 명령 코드를 사용하여 WSAIoctl 함수를 통해 수행됨.   
* Connect, Disconnect, Accept의 확장 메커니즘을 얻기 위해 WSAIoctl 함수에 SIO_GET_EXTENSION_FUNCTION_POINTER 명령코드와 GUID(WSAID_CONNECTEX)을 전달하면 런타임에 확장함수 포인터를 얻을 수 있다.   
* [MSDN ConnectEx 문서](https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nc-mswsock-lpfn_connectex)
* [MSDN AcceptEx 문서](https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex)
* [MSDN DisconnectEx 문서](https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nc-mswsock-lpfn_disconnectex)
