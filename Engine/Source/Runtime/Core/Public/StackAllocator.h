#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct StackAllocator
	{
		char* pMemory;
		char* pCursor;
		U64 allocatedSize;
	};

	StackAllocator CreateStackAllocator(U64 sizeInBytes);
	void* AllocFromStack(StackAllocator* pAllocator, U64 sizeInBytes);
	void FreeBackToStack(StackAllocator* pAllocator, void* ptr);
	void FreeStackAllocator(StackAllocator* pAllocator);
}