#pragma once
#include <PrimitiveTypes.h>

namespace MarkTech
{
	typedef U64 StackMarker;

	class StackAllocator
	{
	public:
		StackAllocator(U64 sizeInBytes, U64 alignment);
		StackAllocator(void* memory, U64 sizeInBytes);
		~StackAllocator();

		void* Alloc(U64 sizeInBytes);

		StackMarker Mark();

		void FreeToMark(StackMarker marker);

		void Clear();

	private:
		U8* m_pAllocatedBytes;
		U8* m_pTopMarker;
		U64 m_AllocatedSize;
		U64 m_UsedSize;
	};
}