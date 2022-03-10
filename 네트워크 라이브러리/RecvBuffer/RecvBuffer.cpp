#include "pch.h"
#include "RecvBuffer.h"

/*--------------
	RecvBuffer
----------------*/

RecvBuffer::RecvBuffer(int32 bufferSize) : _bufferSize(bufferSize)
{
	//실제 데이터 버퍼의 용량 = 단일 버퍼 사이즈 * 버퍼 카운트
	_capacity = bufferSize * BUFFER_COUNT;
	_buffer.resize(_capacity);
}

RecvBuffer::~RecvBuffer()
{
}

void RecvBuffer::Clean()
{
	int32 dataSize = DataSize();
	if (dataSize == 0)
	{
		// 딱 마침 읽기+쓰기 커서가 동일한 위치라면, 둘 다 리셋.
		_readPos = _writePos = 0;
	}
	else
	{
		// 여유 공간이 버퍼 1개 크기 미만이면, 데이터를 앞으로 땅긴다.
		//_bufferSize 단일 버퍼 사이즈
		//최대한 복사 비용을 줄이기 위해 남은 버퍼가 단일 버퍼 사이즈 미만일 경우에만 복사한다.
		if (FreeSize() < _bufferSize)
		{
			//0번 위치에 readPos~writePos 사이의 데이터를 본사한뒤 커서를 옮긴다.
			::memcpy(&_buffer[0], &_buffer[_readPos], dataSize);
			_readPos = 0;
			_writePos = dataSize;
		}
	}
}

bool RecvBuffer::OnRead(int32 numOfBytes)
{
	//유효한 데이터보다 많이 읽은 경우 실패(r 커서 이동)
	if (numOfBytes > DataSize())
		return false;

	//커서 이동
	_readPos += numOfBytes;
	return true;
}

bool RecvBuffer::OnWrite(int32 numOfBytes)
{
	//남아있는 버퍼보다 더 쓸 경우 실패
	if (numOfBytes > FreeSize())
		return false;

	//w커서 이동
	_writePos += numOfBytes;
	return true;
}
