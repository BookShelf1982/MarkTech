#include "Array.h"

template<typename T>
void CTArray<T>::Push(T element)
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
void CTArray<T>::SetAtIndex(T elemet, int index)
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
void CTArray<T>::RemoveAtIndex(int index)
{

}

template<typename T>
T CTArray<T>::GetElementAtIndex(int index)
{
	return arr[index];
}

