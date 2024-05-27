#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	/* 
	* Shift the given address upwards to ensure 
	* it is aligned to the given number of bytes.
	*/
	uintptr AlignAddress(uintptr addr, size_t align);

	/*
	* Shift the given pointer upwards to ensure
	* it is aligned to the given number of bytes.
	*/
	template<typename T>
	inline T* AlignPointer(T* ptr, size_t align)
	{
		const uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
		const uintptr_t addrAligned = AlignAddress(addr, align);
		return reinterpret_cast<T*>(addrAligned);
	}
	
	/*
	* Aligned allocation function. IMPORTANT: 'align'
	* must be a power of 2 (typically 4, 8 or 16).
	*/
	void* AllocAligned(size_t bytes, size_t align);


	/*
	* Frees aligned memory
	*/
	void FreeAligned(void* pMem);

}