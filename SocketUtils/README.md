## 확장 메커니즘
* 확장 기능을 호출하려면 응용 프로그램에서 먼저 원하는 기능에 대한 포인터를 요청해야 한다.
* SIO_GET_EXTENSION_FUNCTION_POINTER 명령 코드를 사용하여 WSAIoctl 함수를 통해 수행됨.
* Connect, Disconnect, Accept의 확장 메커니즘을 얻기 위해 WSAIoctl 함수에 SIO_GET_EXTENSION_FUNCTION_POINTER 명령코드와 GUID(WSAID_CONNECTEX)을 전달하면 런타임에 확장함수 포인터를 얻을 수 있다.
* [ConnectEx](https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nc-mswsock-lpfn_connectex)
* [AcceptEx](https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nf-mswsock-acceptex)
* [DisconnectEx](https://docs.microsoft.com/en-us/windows/win32/api/mswsock/nc-mswsock-lpfn_disconnectex)
