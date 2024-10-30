#pragma once
#include <PrimitiveTypes.h>
#include "LinkedList.h"

namespace MarkTech
{
	struct MemoryBlock
	{
		char* pMem;
		U32 size;
	};

	struct MemoryAllocator
	{
		LinkedList<MemoryBlock> freeMemory;
		LinkedList<MemoryBlock> usedMemory;
		U32 allocatedSize;
		U32 usedSize;
		char* pMemory;
	};

	void CreateMemoryAllocator(MemoryAllocator& allocator, U32 size, PoolAllocator* pPool);
	void* AllocFromMemAllocator(MemoryAllocator& allocator, U32 size);
	void FreeToMemAllocator(MemoryAllocator& allocator, void* ptr, U32 size);
	void DefragMemAllocator(MemoryAllocator& allocator);
	void FreeMemoryAllocator(MemoryAllocator& allocator);
}