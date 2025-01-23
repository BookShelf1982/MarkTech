#pragma once

namespace MarkTech
{
	void* AlignedAlloc(size_t size, size_t alignment);
	void AlignedFree(void* ptr);

	struct AllocatorPfns
	{
		void* (*aligned_malloc_pfn)(size_t size, size_t alignemnt);
		void* (*aligned_free_pfn)(void* ptr);
	};

	inline void* AllocUsingPfns(const AllocatorPfns* pAllocator, size_t size, size_t alignment)
	{
		if (pAllocator)
			return pAllocator->aligned_malloc_pfn(size, alignment);
		else
			return AlignedAlloc(size, alignment);
	}

	inline void FreeUsingPfns(const AllocatorPfns* pAllocator, void* ptr)
	{
		if (pAllocator)
			pAllocator->aligned_free_pfn(ptr);
		else
			AlignedFree(ptr);
	}
}