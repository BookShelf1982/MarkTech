#include "StackAllocator.h"
#include <Assert.h>
#include <memory.h>
#include <stdlib.h>

namespace MarkTech
{
	StackAllocator CreateStackAllocator(U64 sizeInBytes)
	{
		StackAllocator allocator = {};
		return allocator;
	}

	void CreateStackAllocator(StackAllocator* pAlloc, U64 sizeInBytes)
	{
		pAlloc->allocatedSize = sizeInBytes;
		pAlloc->pMemory = (char*)malloc(sizeInBytes);
		memset(pAlloc->pMemory, 0, sizeInBytes);
		pAlloc->pCursor = pAlloc->pMemory;
	}

	void* AllocFromStack(StackAllocator* pAllocator, U64 sizeInBytes)
	{
		MT_ASSERT(pAllocator->allocatedSize > (pAllocator->pCursor - pAllocator->pMemory));
		void* ptr = pAllocator->pCursor;
		pAllocator->pCursor += sizeInBytes;
		return ptr;
	}

	void FreeBackToStack(StackAllocator* pAllocator, void* ptr)
	{
		MT_ASSERT(pAllocator->pMemory <= ptr);
		MT_ASSERT(pAllocator->pCursor >= ptr);
		char* pPrevPtr = pAllocator->pCursor;
		pAllocator->pCursor = (char*)ptr;
		memset(pAllocator->pMemory, 0, (pPrevPtr - pAllocator->pCursor));
	}

	void ClearStack(StackAllocator* pAllocator)
	{
		memset(pAllocator->pMemory, 0, pAllocator->allocatedSize);
	}

	void FreeStackAllocator(StackAllocator* pAllocator)
	{
		free(pAllocator->pMemory);
	}
}