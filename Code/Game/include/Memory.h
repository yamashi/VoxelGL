#pragma once

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

void InitializableMemory();
void ShutdownMemory();

struct MemoryBlock;

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


// MemoryPool assumes any memory allocated can live forever, deleting a memory pool does not free its allocations
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
			s_instance = new (pPlacement) MemoryPool;
		}
		return *s_instance;
	}

private:

	static const uint32_t cPoolCount = 14;

	static MemoryPool* s_instance;

	StaticMemoryPool* m_pPools[cPoolCount];
	std::mutex m_lock;
};

#define MEMORY_PUSH(pool) MemoryPoolStackScope _memoryPoolStack_##__LINE__(pool)

class MemoryPoolStack
{
public:

	void Push(MemoryPool* apPool);
	void Pop();

	MemoryPool* Get() const;

private:

	MemoryPool* m_pPool[64];
	uint32_t m_head{ 0 };
};

struct MemoryPoolStackScope
{
	MemoryPoolStackScope(MemoryPool* apPool);
	~MemoryPoolStackScope();
};

extern MemoryPool* s_pGraphicPool;
extern MemoryPool* s_pNetworkPool;
extern MemoryPool* s_pPhysicsPool;
extern MemoryPool* s_pScratchPool;