#pragma once
#include <PrimitiveTypes.h>

#define MEGABYTE 1048576
#define KILOBYTE 1024

namespace MarkTech
{
	struct MemoryQuery
	{
		U64 allocatedMemory;
		U64 allocatedDebugMemory;
		U64 usedMemory;
		U64 usedDebugMemory;
	};

	class MemoryManager
	{
	public:
		MemoryManager();
		~MemoryManager();

		void Init(U64 sizeInBytes, U64 debugSizeInBytes);
		void Shutdown();

		static void* Alloc(U64 sizeInBytes, U64 alignment);
		void* AllocMem(U64 sizeInBytes, U64 alignment);

		static void* AllocDbg(U64 sizeInBytes, U64 alignment);
		void* AllocMemDbg(U64 sizeInBytes, U64 alignment);

		static MemoryQuery GetMemoryStats();

	private:
		U8* m_pMemory;
		U8* m_pMemCursor;
		U8* m_pDbgMemCursor;
		U64 m_AllocatedMemory;
		U64 m_AllocatedDebugMemory;
		U64 m_UsedMemory;
		U64 m_UsedDebugMemory;

		static MemoryManager* m_gpMemManager;
	};
}