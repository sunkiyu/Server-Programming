#include "pch.h"
#include "Memory.h"
#include "MemoryPool.h"

/*-------------
	Memory
---------------*/

Memory::Memory()
{
	int32 size = 0;
	int32 tableIndex = 0;

	for (size = 32; size <= 1024; size += 32)
	{
		//메모리 풀에 메모리를 할당해서 넣어준다.
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		//0~32번 까지는 1번째 _poolTable 참조
		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (; size <= 2048; size += 128)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}

	for (; size <= 4096; size += 256)
	{
		MemoryPool* pool = new MemoryPool(size);
		_pools.push_back(pool);

		while (tableIndex <= size)
		{
			_poolTable[tableIndex] = pool;
			tableIndex++;
		}
	}
}

Memory::~Memory()
{
	for (MemoryPool* pool : _pools)
		delete pool;

	_pools.clear();
}

void* Memory::Allocate(int32 size)
{
	MemoryHeader* header = nullptr;
	//요청한 사이즈에 메모리 헤더 사이즈 더해준다.
	const int32 allocSize = size + sizeof(MemoryHeader);

	//최대 alloc 사이즈를 벗어나면 일반 할당
	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 할당
		header = reinterpret_cast<MemoryHeader*>(::malloc(allocSize));
	}
	else
	{
		// 메모리 풀에서 꺼내온다
		header = _poolTable[allocSize]->Pop();
	}

	return MemoryHeader::AttachHeader(header, allocSize);
}

void Memory::Release(void* ptr)
{
	MemoryHeader* header = MemoryHeader::DetachHeader(ptr);

	const int32 allocSize = header->allocSize;
	//0이하면 crash
	ASSERT_CRASH(allocSize > 0);

	if (allocSize > MAX_ALLOC_SIZE)
	{
		// 메모리 풀링 최대 크기를 벗어나면 일반 해제
		::free(header);
	}
	else
	{
		// 메모리 풀에 반납한다
		_poolTable[allocSize]->Push(header);
	}
}
