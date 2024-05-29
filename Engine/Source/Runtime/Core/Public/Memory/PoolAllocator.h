#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	struct  FreeElement
	{
		U8* pBlock;
		FreeElement* pNext;
	};

	class PoolAllocator
	{
	public:
		PoolAllocator(U32 sizeInBytes, U32 blockSizeInBytes, U32 alignment);
		~PoolAllocator();

		void* Alloc();

		void Free(void* ptr);
	private:
		U8* m_pMemory;
		FreeElement* m_pFreeElements;
		FreeElement* m_pLinkStart;
		U32 m_ElementsSize;
		U32 m_NumOfElements;
	};
}