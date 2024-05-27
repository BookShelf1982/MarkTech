#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	class StackAllocator
	{
	public:
		explicit StackAllocator(U32 sizeInBytes, U32 alignment);
		~StackAllocator();

		void* Alloc(U32 sizeInBytes);

		void Mark();

		void FreeToMark();

		void Clear();

	private:
		U8* m_pAllocatedBytes;
		U8* m_pTopMarker;
		U8* m_pRollbackMarker;
		U32 m_AllocatedSize;
	};
}