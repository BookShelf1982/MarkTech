#include "StackAllocator.h"
#include <Assert.h>
#include <memory.h>
#include <stdlib.h>

namespace MarkTech
{
	StackAllocator CreateStackAllocator(U64 sizeInBytes)
	{
		StackAllocator allocator = {};
		allocator.allocatedSize = sizeInBytes;
		allocator.pMemory = (char*)malloc(sizeInBytes);
		memset(allocator.pMemory, 0, sizeInBytes);
		allocator.pCursor = allocator.pMemory;
		return allocator;
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

	void FreeStackAllocator(StackAllocator* pAllocator)
	{
		free(pAllocator->pMemory);
	}
}