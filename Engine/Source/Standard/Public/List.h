#pragma once
#include "AllocatorType.h"

template<typename T, class A = CAllocator>
class CTList
{
public:
	CTList();
	CTList(unsigned int nSize);
	~CTList();

	void Push(T element);
	uint32_t Size() { return m_nSize; }
	T* Data() { return m_pList; }
private:
	T* m_pList;
	unsigned int m_nSize;
	unsigned int m_nMaxSize;
	A m_Allocator;
};

template<typename T, class A>
inline CTList<T, A>::CTList()
	:m_pList(nullptr), m_nSize(0), m_nMaxSize(0)
{
}

template<typename T, class A>
inline CTList<T, A>::CTList(unsigned int nSize)
	:m_pList(nullptr), m_nSize(0), m_nMaxSize(nSize)
{
	m_pList = (T*)m_Allocator.Allocate(nSize * sizeof(T));
	m_Allocator.ConstructObject<T>(m_pList, nSize);
}

template<typename T, class A>
inline CTList<T, A>::~CTList()
{
	for (unsigned int i = 0; i < m_nSize; i++)
	{
		m_pList[i].~T();
	}
	m_Allocator.Deallocate(m_pList);
}

template<typename T, class A>
inline void CTList<T, A>::Push(T element)
{
	if (m_nSize != m_nMaxSize)
	{
		m_pList[m_nSize] = element;
		m_nSize++;
	}
}
