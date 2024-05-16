#include "Allocator.h"
#include <stdlib.h>
#include <string.h>

namespace MarkTech
{
	Allocator::Allocator()
	{
	}

	Allocator::~Allocator()
	{
	}

	void* Allocator::Allocate(U32 sizeInBytes)
	{
		void* ptr = malloc((size_t)sizeInBytes);
		memset(ptr, 0, sizeInBytes);
		return ptr;
	}

	void Allocator::Free(void* ptr)
	{
		free(ptr);
	}
}