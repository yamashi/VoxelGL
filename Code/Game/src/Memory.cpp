#include "Memory.h"
#include "Utils.h"
#include "ThreadLocal.h"

// This will leak on purpose
MemoryPool* MemoryPool::s_instance = nullptr;

MemoryPool* s_pGraphicPool = nullptr;
MemoryPool* s_pNetworkPool = nullptr;
MemoryPool* s_pPhysicsPool = nullptr;
MemoryPool* s_pScratchPool = nullptr;

void* operator new(std::size_t n)
{
	void* p = nullptr;

	if (s_pTls)
	{
		MemoryPool* pPool = s_pTls->memory.Get();
		if (pPool)
		{
			p = pPool->Allocate((uint32_t)n);
		}
	}

	if (!p)
	{
		p = MemoryPool::GetInstance().Allocate((uint32_t)n);
	}

	return p;
}

void operator delete(void * p) throw()
{
	MemoryPool::GetInstance().Free(p);
}

void *operator new[](std::size_t n)
{
	void* p = nullptr;

	if (s_pTls)
	{
		MemoryPool* pPool = s_pTls->memory.Get();
		if (pPool)
		{
			p = pPool->Allocate((uint32_t)n);
		}
	}

	if (!p)
	{
		p = MemoryPool::GetInstance().Allocate((uint32_t)n);
	}

	return p;
}
void operator delete[](void *p) throw()
{
	MemoryPool::GetInstance().Free(p);
}

template<int> uint32_t _GetExponentShift();
template<> uint32_t _GetExponentShift<4>()
{
	return 2;
}
template<> uint32_t _GetExponentShift<8>()
{
	return 3;
}

inline uint32_t GetExponentShift() { return _GetExponentShift<sizeof(void*)>(); }

void InitializableMemory()
{
	s_pGraphicPool = new MemoryPool;
	s_pNetworkPool = new MemoryPool;
	s_pPhysicsPool = new MemoryPool;
	s_pScratchPool = new MemoryPool;
}

void ShutdownMemory()
{
	delete s_pScratchPool;
	delete s_pPhysicsPool;
	delete s_pNetworkPool;
	delete s_pGraphicPool;
}

StaticMemoryPool::StaticMemoryPool(uint32_t aExponent)
	: m_dataSize(1 << aExponent)
{
	m_availableMax = 0;

	intptr_t alignOffset = 6 * sizeof(void*);
	intptr_t startAddr = intptr_t(0) + alignOffset;

	while (alignOffset + m_dataSize < cBlockSize)
	{
		alignOffset += m_dataSize;
		m_availableMax++;
	}

	m_pHead = CreateBlock();
	m_pCurrent = m_pHead;
}

StaticMemoryPool::~StaticMemoryPool()
{
}

MemoryBlock* StaticMemoryPool::CreateBlock()
{
	MemoryBlock* pBlock = (MemoryBlock*)AllocAligned(cBlockSize, cBlockSize);
	pBlock->magic = cMagic;
	pBlock->pNextBlock = nullptr;
	pBlock->pParent = this;
	pBlock->available = m_availableMax;

	intptr_t alignOffset = 6 * sizeof(void*);
	intptr_t startAddr = intptr_t(pBlock) + alignOffset;
	MemoryBlock::Data* pCurrent = (MemoryBlock::Data*)startAddr;
	pBlock->pHead = pCurrent;

	while (alignOffset + m_dataSize < cBlockSize)
	{
		pCurrent->pNext = (MemoryBlock::Data*)(intptr_t(pBlock) + alignOffset);
		pCurrent = pCurrent->pNext;
		alignOffset += m_dataSize;
	}

	pCurrent->pNext = nullptr;

	return pBlock;
}

void* StaticMemoryPool::Allocate()
{
	m_lock.lock();

	MemoryBlock::Data* pCurrent = m_pCurrent->pHead;
	if (pCurrent == nullptr)
	{
		FindBlock();
		pCurrent = m_pCurrent->pHead;
	}

	m_pCurrent->pHead = pCurrent->pNext;
	m_pCurrent->available--;

	m_lock.unlock();

	return (void*)pCurrent;

}

void StaticMemoryPool::Collect(MemoryBlock* apBlock, void* apPtr)
{
	if (apBlock == nullptr || this == nullptr)
	{
		return;
	}

	m_lock.lock();

	MemoryBlock::Data* pCurrent = (MemoryBlock::Data*)apPtr;
	MemoryBlock::Data* pHead = apBlock->pHead;

	apBlock->pHead = pCurrent;
	pCurrent->pNext = pHead;

	if (apBlock == m_pHead || m_pCurrent == apBlock)
	{
		m_lock.unlock();

		return;
	}

	if (apBlock->available == m_availableMax)
	{
		FreeAligned(apBlock);
	}

	m_lock.unlock();
}

void StaticMemoryPool::FindBlock()
{
	MemoryBlock* pPrevious = m_pHead;
	MemoryBlock* pCurrent = m_pHead;

	while (pCurrent != nullptr && pCurrent->pHead == nullptr)
	{
		pPrevious = pCurrent;
		pCurrent = pCurrent->pNextBlock;
	}

	if (pCurrent == nullptr)
	{
		m_pCurrent = CreateBlock();
		pPrevious->pNextBlock = m_pCurrent;
	}
	else
	{
		m_pCurrent = pCurrent;
	}
}

MemoryPool::MemoryPool()
{
	for (uint32_t i = 0; i < cPoolCount; ++i)
	{
		void* pInplace = malloc(sizeof(StaticMemoryPool) * 2);
		m_pPools[i] = new (pInplace) StaticMemoryPool(i + GetExponentShift());
	}
}

MemoryPool::~MemoryPool()
{
}

void* MemoryPool::Allocate(uint32_t aSize)
{
	uint32_t roundedSize = NextPower2(aSize);
	roundedSize = roundedSize < sizeof(void*) ? 1 << sizeof(void*) : roundedSize;
	uint32_t exponent = TrailingZeroes(roundedSize) - GetExponentShift();

	if (exponent < cPoolCount)
	{
		return m_pPools[exponent]->Allocate();
	}

	// We need to find a cleaner way to do this
	return nullptr;
}

void MemoryPool::Free(void* apPtr)
{
	if (apPtr == nullptr) return;

	intptr_t blockPtr = (intptr_t)apPtr;
	blockPtr &= StaticMemoryPool::cBlockMask;

	MemoryBlock* pBlock = (MemoryBlock*)blockPtr;

	// This should never happen
	if (pBlock->magic != StaticMemoryPool::cMagic)
	{
		exit(-2);
	}

	StaticMemoryPool* pPool = pBlock->pParent;

	if (pPool)
	{
		pPool->Collect(pBlock, apPtr);
	}
}

void MemoryPoolStack::Push(MemoryPool* apPool)
{
	m_pPool[m_head] = apPool;
	m_head++;
}

void MemoryPoolStack::Pop()
{
	m_head--;
}

MemoryPool* MemoryPoolStack::Get() const
{
	if (m_head != 0)
	{
		return m_pPool[m_head - 1];
	}

	return nullptr;
}

MemoryPoolStackScope::MemoryPoolStackScope(MemoryPool* apPool)
{
	s_pTls->memory.Push(apPool);
}

MemoryPoolStackScope::~MemoryPoolStackScope()
{
	s_pTls->memory.Pop();
}