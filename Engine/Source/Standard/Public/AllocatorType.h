#pragma once
#include <stdlib.h>
#include <string.h>


class CAllocator
{
public:
	CAllocator();
	~CAllocator();

	void* Allocate(unsigned int nSizeInBytes);
	void Deallocate(void* pMemory);

	template<typename T>
	T* ConstructObject(T* pMemory, unsigned int nNumObjects);
private:

};

template<typename T>
inline T* CAllocator::ConstructObject(T* pMemory, unsigned int nNumObjects)
{
	for (unsigned int i = 0; i < nNumObjects; i++)
	{
		T object;
		memcpy(&pMemory[i * sizeof(T)], &object, sizeof(T));
	}

	return pMemory;
}
