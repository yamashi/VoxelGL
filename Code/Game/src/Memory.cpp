#include "Memory.h"
#include "Utils.h"

#include <memory>
#include <iostream>


void* operator new(std::size_t n)
{
	void* p = MemoryPool::GetInstance().Allocate((uint32_t)n);
	//std::cout << "alloc " << p << std::endl;
	return p;
}

void operator delete(void * p) throw()
{
	//std::cout << "delete " << p << std::endl;
	MemoryPool::GetInstance().Free(p);
}

void *operator new[](std::size_t n)
{
	void* p = MemoryPool::GetInstance().Allocate((uint32_t)n);
	//std::cout << "alloc[] " << p << std::endl;
	return p;
}
void operator delete[](void *p) throw()
{
	//std::cout << "delete[] " << p << std::endl;
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


StaticMemoryPool::StaticMemoryPool(uint32_t aExponent)
	: m_dataSize(1 << aExponent)
{
	m_pHead = CreateBlock();
	m_pCurrent = m_pHead;
	m_availableMax = 0;

	intptr_t alignOffset = 6 * sizeof(void*);
	intptr_t startAddr = intptr_t(0) + alignOffset;

	while (alignOffset + m_dataSize < cBlockSize)
	{
		alignOffset += m_dataSize;
		m_availableMax++;
	}
}

StaticMemoryPool::~StaticMemoryPool()
{
	MemoryBlock* pNext = nullptr;
	while (m_pHead)
	{
		pNext = m_pHead->pNextBlock;
		FreeAligned(m_pHead);
		m_pHead = pNext;
	}
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
		pBlock->available++;
	}

	pCurrent->pNext = nullptr;

	return pBlock;
}

void* StaticMemoryPool::Allocate()
{
	MemoryBlock::Data* pCurrent = m_pCurrent->pHead;
	if (pCurrent == nullptr)
	{
		FindBlock();
		return Allocate();
	}

	m_pCurrent->pHead = pCurrent->pNext;
	m_pCurrent->available--;

	return (void*)pCurrent;
}

void StaticMemoryPool::Collect(MemoryBlock* apBlock)
{
	if (apBlock == m_pHead || m_pCurrent == apBlock)
	{
		return;
	}

	if (apBlock->available == m_availableMax)
	{
		FreeAligned(apBlock);
	}
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
		void* pInplace = malloc(sizeof(StaticMemoryPool));
		m_pPools[i] = new (pInplace) StaticMemoryPool(i + GetExponentShift());
	}
}

MemoryPool::~MemoryPool()
{
	for (uint32_t i = 0; i < cPoolCount; ++i)
	{
		free(m_pPools[i]);
	}
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
	intptr_t blockPtr = (intptr_t)apPtr;
	blockPtr &= StaticMemoryPool::cBlockMask;

	MemoryBlock* pBlock = (MemoryBlock*)blockPtr;

	// This should never happen
	if (pBlock->magic != StaticMemoryPool::cMagic)
	{
		exit(-2);
	}

	StaticMemoryPool* pPool = pBlock->pParent;

	MemoryBlock::Data* pCurrent = (MemoryBlock::Data*)apPtr;
	MemoryBlock::Data* pHead = pBlock->pHead;

	pBlock->pHead = pCurrent;
	pCurrent->pNext = pHead;

	pPool->Collect(pBlock);
}