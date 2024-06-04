#pragma once
#include "Memory\MemoryManager.h"
#include "Memory\StackAllocator.h"

namespace MarkTech
{
	class DebugStringManager
	{
	public:
		DebugStringManager();
		~DebugStringManager();

		void Init(void* ptr, U64 memorySize);
		void Shutdown();
		void* AllocMem(U64 sizeInBytes);

		static void* AllocStringDbg(U64 sizeInBytes);
	private:
		StackAllocator m_StringAlloc;

		static DebugStringManager* m_gpStringAllocator;
	};
}