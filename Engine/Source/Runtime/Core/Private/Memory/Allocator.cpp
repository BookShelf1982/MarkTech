#include "Memory\Allocator.h"
#include "Memory\AlignedAllocator.h"
#include <stdlib.h>

namespace MarkTech
{
	Allocator::Allocator()
	{
	}

	Allocator::~Allocator()
	{
	}

	void* Allocator::Alloc(U32 sizeInBytes, U32 alignment)
	{
		void* ptr = AllocAligned((size_t)sizeInBytes, (size_t)alignment);
		return ptr;
	}

	void* Allocator::Allocate(U32 sizeInBytes)
	{
		return malloc(sizeInBytes);
	}

	void Allocator::Free(void* ptr)
	{
		free(ptr);
	}
}