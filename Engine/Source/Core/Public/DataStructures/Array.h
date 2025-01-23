#pragma once
#include "PrimitiveTypes\intalias.h"

#include <stdlib.h>

namespace MarkTech
{
	template<typename T, size_t S>
	struct FixedArray
	{
		T a[S];
		size_t length;
	};

	template<typename T, size_t S>
	inline void InsertArrayItem(FixedArray<T, S>& array, T item)
	{
		array.a[array.length] = item;
		array.length++;
	}

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