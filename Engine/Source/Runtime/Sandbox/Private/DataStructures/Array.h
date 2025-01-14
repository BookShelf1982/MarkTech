#pragma once
#include "PrimitiveTypes\/intalias.h"

#include <stdlib.h>

namespace MarkTech
{
	template<typename T>
	struct DynamicArray
	{
		T* pArray;
		U32 size;
		U32 reservedSize;
	};

	template<typename T>
	DynamicArray<T> ReserveArray(U32 size)
	{
		DynamicArray<T> array = {};
		array.pArray = (T*)malloc(sizeof(T) * size);
		array.reservedSize = size;
		return array;
	}

	template<typename T>
	void InsertArrayItem(DynamicArray<T>& array, T item)
	{
		if (array.size + 1 > array.reservedSize)
			return;

		array.pArray[array.size] = item;
		array.size++;
	}

	template<typename T>
	void DestroyArray(DynamicArray<T> array)
	{
		free(array.pArray);
	}
}