#pragma once

#include <cstdint>
#include <mutex>

#ifdef WIN32
#	define StackAllocateArray(type, c) (type*)_malloca(sizeof(type) * c)
#	define StackAllocate(type) StackAllocateArray(type, 1)
#else
#	define StackAllocateArray(type, c) (type*)alloca(sizeof(type) * c)
#	define StackAllocate(type) StackAllocateArray(type, 1)
#endif

#ifdef WIN32
#	define AllocAligned(size, alignment) _aligned_malloc(size, alignment)
#	define FreeAligned(ptr) _aligned_free(ptr)
#endif

struct MemoryBlock
{
	uint32_t magic;
	uint32_t available;
	class StaticMemoryPool* pParent;
	MemoryBlock* pNextBlock;

	union Data
	{
		Data* pNext;
		void* pData;
	};

	Data* pHead;
};

class StaticMemoryPool
{
public:

	StaticMemoryPool(uint32_t aExponent);
	~StaticMemoryPool();

	void* Allocate();
	void Free(void* apPtr);

	void Collect(MemoryBlock* apBlock, void* apPtr);

	static const uint32_t cMagic = 0xDEAFBEEF;
	static const uint32_t cBlockSize = 1 << 22;
	static const intptr_t cBlockMask = ~(intptr_t(cBlockSize) - 1);

private:

	MemoryBlock* CreateBlock();

	void FindBlock();

	MemoryBlock* m_pHead;
	MemoryBlock* m_pCurrent;

	uint32_t m_dataSize;
	uint32_t m_availableMax;

	std::mutex m_lock;
};

class MemoryPool
{
public:

	MemoryPool();
	~MemoryPool();

	void* Allocate(uint32_t aSize);
	void Free(void* apPtr);

	static MemoryPool& GetInstance()
	{
		if (s_instance == nullptr)
		{
			void* pPlacement = malloc(sizeof(MemoryPool));
			s_instance = new (pPlacement)MemoryPool;
		}
		return *s_instance;
	}

private:

	static const uint32_t cPoolCount = 14;

	static MemoryPool* s_instance;

	StaticMemoryPool* m_pPools[cPoolCount];
	std::mutex m_lock;
};