#pragma once
#include "DllExport.h"

template<typename T>
class MARKTECH_API CTArray
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

	int GetSize() { return size_current; }
	int GetMaxSize() { return size_max; }
private:
	T* arr;
	int size_current;
	int size_max;
};