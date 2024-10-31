#pragma once
#include <PrimitiveTypes.h>
#include "MemoryAllocator.h"
#include <stdlib.h>

namespace MarkTech
{
	template<typename T>
	struct Array
	{
		T* pArray;
		MemoryAllocator* pAlloc;
		U32 size;
		U32 reservedSize;
	};
	
	template<typename T>
	void CreateArray(Array<T>& arr, MemoryAllocator* pAlloc)
	{
		if (pAlloc)
		{
			arr.pArray = (T*)AllocFromMemAllocator(*pAlloc, sizeof(T) * 1);
			arr.pAlloc = pAlloc;
		}
		else
		{
			arr.pArray = (T*)malloc(sizeof(T) * 1);
			arr.pAlloc = nullptr;
		}

		arr.reservedSize = 1;
		arr.size = 0;
	}

	template<typename T>
	void ReserveArray(Array<T>& arr, U32 size, MemoryAllocator* pAlloc)
	{
		if (pAlloc)
		{
			arr.pArray = (T*)AllocFromMemAllocator(*pAlloc, sizeof(T) * size);
			arr.pAlloc = pAlloc;
		}
		else
		{
			arr.pArray = (T*)malloc(sizeof(T) * size);
			arr.pAlloc = nullptr;
		}

		arr.reservedSize = size;
		arr.size = 0;
	}

	template<typename T>
	void InsertArrayItem(Array<T>& arr, T item)
	{
		if (arr.size + 1 > arr.reservedSize)
		{
			T* pNewArray = nullptr;
			if (arr.pAlloc)
			{
				pNewArray = (T*)AllocFromMemAllocator(*arr.pAlloc, (sizeof(T) * arr.reservedSize) * 2);
			}
			else
			{
				pNewArray = (T*)malloc((sizeof(T) * arr.reservedSize) * 2);
			}

			for (U32 i = 0; i < arr.size; i++)
			{
				pNewArray[i] = arr.pArray[i];
			}

			if (arr.pAlloc)
				FreeToMemAllocator(*arr.pAlloc, arr.pArray, arr.reservedSize);
			else
				free(arr.pArray);

			arr.pArray = pNewArray;
			arr.reservedSize *= 2;
		}

		arr.pArray[arr.size] = item;
		arr.size++;
	}

	template<typename T>
	void RemoveArrayItemAtIndex(Array<T>& arr, U32 index)
	{
		for (U32 i = index; i < arr.size; i++)
		{
			arr.pArray[i] = arr.pArray[i + 1];
		}
		arr.size--;
	}

	template<typename T>
	void DestroyArray(Array<T>& arr)
	{
		if (arr.pAlloc)
			FreeToMemAllocator(*arr.pAlloc, arr.pArray, arr.reservedSize);
		else
			free(arr.pArray);
	}
}