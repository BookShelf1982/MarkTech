#include "MemoryAllocator.h"

namespace MarkTech
{
	void CreateMemoryAllocator(MemoryAllocator& allocator, U32 size, PoolAllocator* pPool)
	{
		allocator.allocatedSize = size;
		allocator.usedSize = 0;
		allocator.pMemory = (char*)malloc(size);
		CreateLinkedList(allocator.freeMemory, pPool);
		CreateLinkedList(allocator.usedMemory, pPool);
		InsertLinkedListItem(allocator.freeMemory, { allocator.pMemory, size });
	}

	void* AllocFromMemAllocator(MemoryAllocator& allocator, U32 size)
	{
		LinkedList<MemoryBlock>::NodeType* pNode = allocator.freeMemory.pStart;
		LinkedList<MemoryBlock>::NodeType* pBestFit = pNode;
		while (pNode != nullptr)
		{
			if (pNode->data.size < size)
				continue;

			if (pNode->data.size == size)
			{
				pBestFit = pNode;
				void* ptr = pBestFit->data.pMem;
				MemoryBlock usedMemory;
				usedMemory.pMem = (char*)ptr;
				usedMemory.size = size;

				InsertLinkedListItem(allocator.usedMemory, usedMemory);
				RemoveLinkedListItem(allocator.freeMemory, pBestFit);
				return ptr;
			}

			if (pNode->data.size < pBestFit->data.size)
			{
				pBestFit = pNode;
			}

			pNode = pNode->pNext;
		}

		if (!pBestFit)
		{
			if (allocator.allocatedSize - allocator.usedSize >= size)
			{
				DefragMemAllocator(allocator);
			}
		}

		void* ptr = pBestFit->data.pMem;

		MemoryBlock leftoverMemory;
		leftoverMemory.pMem = ((char*)ptr) + size;
		leftoverMemory.size = pBestFit->data.size - size;

		MemoryBlock usedMemory;
		usedMemory.pMem = (char*)ptr;
		usedMemory.size = size;

		InsertLinkedListItem(allocator.freeMemory, leftoverMemory);
		InsertLinkedListItem(allocator.usedMemory, usedMemory);
		RemoveLinkedListItem(allocator.freeMemory, pBestFit);

		allocator.usedSize += size;

		return ptr;
	}

	void FreeToMemAllocator(MemoryAllocator& allocator, void* ptr, U32 size)
	{
		MemoryBlock memory;
		memory.pMem = (char*)ptr;
		memory.size = size;

		allocator.usedSize -= size;

		InsertLinkedListItem(allocator.freeMemory, memory);
		
		LinkedList<MemoryBlock>::NodeType* pNode = allocator.usedMemory.pStart;
		while (pNode->data.pMem != ptr)
		{
			pNode = pNode->pNext;
		}
		RemoveLinkedListItem(allocator.usedMemory, pNode);
	}

	void DefragMemAllocator(MemoryAllocator& allocator)
	{
	}

	void FreeMemoryAllocator(MemoryAllocator& allocator)
	{
		DestroyLinkedList(allocator.freeMemory);
		DestroyLinkedList(allocator.usedMemory);
		free(allocator.pMemory);
	}
}