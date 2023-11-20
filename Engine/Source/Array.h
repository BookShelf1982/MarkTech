#pragma once
#include "DllExport.h"

template<typename T>
class CTArray
{
public:
	CTArray()
	{
		arr = new T[1];
		size_max = 1;
		size_current = 0;
	}

	~CTArray()
	{
		delete[] arr;
	}

	void Push(T element);
	void SetAtIndex(T elemet, int index);
	void RemoveAtIndex(int index);
	T GetElementAtIndex(int index);

	T operator[](int index);

	int GetSize() { return size_current; }
	int GetMaxSize() { return size_max; }
	T* c_arr() { return arr; }
private:
	T* arr;
	int size_current;
	int size_max;
};


template<typename T>
inline void CTArray<T>::Push(T element)
{
	//If max size is equal to current size then double size
	if (size_current >= size_max)
	{
		T* tempArr = new T[2 * size_max];
		for (int i = 0; i < size_max; i++)
		{
			tempArr[i] = arr[i];
		}
		delete[] arr;
		size_max *= 2;
		arr = tempArr;
	}

	// Insert element
	arr[size_current] = element;
	size_current++;
}

template<typename T>
inline void CTArray<T>::SetAtIndex(T elemet, int index)
{
	if (index >= size_max)
	{
		Push(elemet);
	}
	else
	{
		arr[index] = elemet;
	}
}

template<typename T>
inline void CTArray<T>::RemoveAtIndex(int index)
{

}

template<typename T>
inline T CTArray<T>::GetElementAtIndex(int index)
{
	return arr[index];
}

template<typename T>
inline T CTArray<T>::operator[](int index)
{
	return arr[index];
}



#define MAKE_CTARRAY_COMPATIBLE(Type) MARKTECH_EXTERN template class MARKTECH_API CTArray<Type>