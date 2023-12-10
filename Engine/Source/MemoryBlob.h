#pragma once
#include "DllExport.h"

template<typename T>
class CTMemoryBlob
{
public:
	CTMemoryBlob();
	~CTMemoryBlob();

	void Initialize();
	void Initialize(size_t size);
	void Release();

	T* GetPtr() const { return ptr; }

	void operator=(T* blob);
private:
	T* ptr;
	bool bAlreadyInit;
	bool bArray;
};

template<typename T>
inline CTMemoryBlob<T>::CTMemoryBlob()
	:ptr(nullptr), bAlreadyInit(false), bArray(false)
{
}

template<typename T>
inline CTMemoryBlob<T>::~CTMemoryBlob()
{
}

template<typename T>
inline void CTMemoryBlob<T>::Initialize()
{
	ptr = new T;
	bAlreadyInit = true;
	bArray = false;
}

template<typename T>
inline void CTMemoryBlob<T>::Initialize(size_t size)
{
	ptr = new T[size];
	bAlreadyInit = true;
	bArray = true;
}

template<typename T>
inline void CTMemoryBlob<T>::Release()
{
	if (bArray)
		delete[] ptr;
	else
		delete ptr;
}

template<typename T>
inline void CTMemoryBlob<T>::operator=(T* blob)
{
	if (!bAlreadyInit)
	{
		ptr = blob;
		bAlreadyInit = true;
	}
}
