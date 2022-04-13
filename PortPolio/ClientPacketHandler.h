```cpp
#pragma once
#include "BufferReader.h"
#include "BufferWriter.h"
enum
{
	S_TEST = 1,
	S_CHAT = 2
};

class ClientPacketHandler
{
public:
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_S_TEST(BYTE* buffer, int32 len);
	static void Handle_S_CHAT(BYTE* buffer, int32 len);
};


template<typename T, typename C>
class PacketIterator
{
public:
	PacketIterator(C& container, uint16 index) : _container(container), _index(index) { }

	bool				operator!=(const PacketIterator& other) const { return _index != other._index; }
	const T&			operator*() const { return _container[_index]; }
	T&					operator*() { return _container[_index]; }
	T*					operator->() { return &_container[_index]; }
	PacketIterator&		operator++() { _index++; return *this; }
	PacketIterator		operator++(int32) { PacketIterator ret = *this; ++_index; return ret; }

private:
	C&				_container;
	uint16			_index;
};

template<typename T>
class PacketList
{
public:
	PacketList() : _data(nullptr), _count(0) { }
	PacketList(T* data, uint16 count) : _data(data), _count(count) { }

	T& operator[](uint16 index)
	{
		ASSERT_CRASH(index < _count);
		return _data[index];
	}

	uint16 Count() { return _count; }

	// ranged-base for 지원
	PacketIterator<T, PacketList<T>> begin() { return PacketIterator<T, PacketList<T>>(*this, 0); }
	PacketIterator<T, PacketList<T>> end() { return PacketIterator<T, PacketList<T>>(*this, _count); }

private:
	T*			_data;
	uint16		_count;
};

#pragma pack(1)
class PKT_S_CHAT_WRITE
{
public:
	struct PKT_S_CHAT
	{
		uint16 packetSize; // 공용 헤더
		uint16 packetId; // 공용 헤더
		uint64 id; // 8
	};

	PKT_S_CHAT_WRITE(uint64 id)
	{
		_sendBuffer = GSendBufferManager->Open(4096);
		_bw = BufferWriter(_sendBuffer->Buffer(), _sendBuffer->AllocSize());
		_pkt = _bw.Reserve<PKT_S_CHAT>();
		_pkt->packetSize = 0; // To Fill
		_pkt->packetId = S_CHAT;
		_pkt->id = id;
	}

	BYTE* ReserveChatMsg(int msgSz)
	{
		BYTE* str = _bw.Reserve<BYTE>(msgSz);
		return str;
	}


	SendBufferRef CloseAndReturn()
	{
		// 패킷 사이즈 계산
		_pkt->packetSize = _bw.WriteSize();
		_sendBuffer->Close(_bw.WriteSize());
		return _sendBuffer;
	}

private:
	PKT_S_CHAT* _pkt = nullptr;
	SendBufferRef _sendBuffer;
	BufferWriter _bw;
	string str;
};
#pragma pack()
```
