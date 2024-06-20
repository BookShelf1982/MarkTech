#pragma once
#include <PrimitiveTypes.h>
#include "Memory\PoolAllocator.h"

namespace MarkTech
{
	class DSManager
	{
	public:
		DSManager();
		~DSManager();

		void Init(U32 blockCount, U32 allocatorCount);
		void Shutdown();

		void* Alloc(U64 blockSize);
		void Free(void* ptr, U64 blockSize);

		static void* AllocBlock(U64 blockSize);
		static void FreeBlock(void* ptr, U64 blockSize);

	private:
		PoolAllocator* m_pPoolAllocs;
		U32 m_AllocatorCount;

		// Static instance
		static DSManager* m_gpDSManager;
	};
}