#pragma once
#include "AllocatorType.h"
#include <stdio.h>

template<typename T, class A = CAllocator>
class CTList
{
public:
	CTList();
	CTList(unsigned int nSize);
	CTList(const CTList& list);
	~CTList();

	void Push(T element);
	void Reserve(unsigned int count);
	unsigned int Size() const { return m_nSize; }
	T* Data() const { return m_pList; }

	T operator[](unsigned int index) const { return m_pList[index]; }

	void operator=(const CTList& list);
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
	: m_pList(nullptr), m_nSize(0), m_nMaxSize(nSize)
{
	m_pList = (T*)m_Allocator.Allocate(nSize * sizeof(T));
	m_Allocator.ConstructObject<T>(m_pList, nSize);
}

template<typename T, class A>
inline CTList<T, A>::CTList(const CTList& list)
	:m_pList(nullptr), m_nMaxSize(list.m_nMaxSize), m_nSize(list.m_nSize)
{
	m_pList = (T*)m_Allocator.Allocate(list.m_nMaxSize * sizeof(T));
	for (unsigned int i = 0; i < m_nMaxSize; i++)
	{
		m_pList[i] = list.m_pList[i];
	}
}

template<typename T, class A>
inline CTList<T, A>::~CTList()
{
	for (unsigned int i = 0; i < m_nMaxSize; i++)
	{
		m_pList[i].~T();
	}
	m_Allocator.Deallocate(m_pList);
}

template<typename T, class A>
inline void CTList<T, A>::Push(T element)
{
	printf("Element pushed!\n");
	if (m_nSize >= m_nMaxSize)
	{
		// Allocate new array
		m_nMaxSize += 1;
		T* pNewArray = (T*)m_Allocator.Allocate(m_nMaxSize * sizeof(T));
		for (unsigned int i = 0; i < m_nSize; i++)
		{
			pNewArray[i] = m_pList[i];
		}

		for (unsigned int i = 0; i < m_nSize; i++)
		{
			m_pList[i].~T();
		}
		m_Allocator.Deallocate(m_pList); // Free old array

		m_pList = pNewArray; // set array to new array
	}

	m_pList[m_nSize] = element;
	m_nSize++;
}

template<typename T, class A>
inline void CTList<T, A>::Reserve(unsigned int count)
{
	if (count > m_nSize)
	{
		// Allocate new array
		T* pNewArray = (T*)m_Allocator.Allocate(count * sizeof(T));
		m_Allocator.ConstructObject<T>(pNewArray, m_nSize);
		memcpy(pNewArray, m_pList, m_nSize * sizeof(T)); // Copy data from previous to new

		m_Allocator.Deallocate(m_pList); // Free old array

		m_pList = pNewArray; // set array to new array
	}
}

template<typename T, class A>
inline void CTList<T, A>::operator=(const CTList& list)
{
	// Destruct and deallocate current array
	for (unsigned int i = 0; i < m_nMaxSize; i++)
	{
		m_pList[i].~T();
	}
	m_Allocator.Deallocate(m_pList);

	// Copy array into our array
	m_nSize = list.m_nSize;
	m_nMaxSize = list.m_nSize;
	m_pList = (T*)m_Allocator.Allocate(list.m_nMaxSize * sizeof(T));
	for (unsigned int i = 0; i < m_nMaxSize; i++)
	{
		m_pList[i] = list.m_pList[i];
	}
}