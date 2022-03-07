#pragma once

/*--------------
	RecvBuffer
----------------*/
//tcp는 데이터의 경계가 없어 Recv하였을 때 그 데이터가 온전히 상대방이 보낸 데이터라는 보장이 없어
//RecvBuffer 클래스로 다룬다.
class RecvBuffer
{
	enum { BUFFER_COUNT = 10 };

public:
	RecvBuffer(int32 bufferSize);
	~RecvBuffer();

	void			Clean();
	bool			OnRead(int32 numOfBytes);
	bool			OnWrite(int32 numOfBytes);

	BYTE*			ReadPos() { return &_buffer[_readPos]; }
	BYTE*			WritePos() { return &_buffer[_writePos]; }
	//유효한 데이터
	int32			DataSize() { return _writePos - _readPos; }
	//남아있는 사이즈
	int32			FreeSize() { return _capacity - _writePos; }

private:
	int32			_capacity = 0;
	int32			_bufferSize = 0;
	//커서 역할을 한다.
	//3 바이트를 쓴 경우의 예
	//w 커서가 3바이트 앞으로 갔다
	//r 커서는 현재 위치부터 이동하며 계속 읽는다.
	//사실상 w와 r 사이가 유효한 버퍼이다.
	//[r][][][w][][][][]
	//보통 처음과 끝이 같은 버퍼를 사용한다.
	int32			_readPos = 0;
	int32			_writePos = 0;
	Vector<BYTE>	_buffer;
};

