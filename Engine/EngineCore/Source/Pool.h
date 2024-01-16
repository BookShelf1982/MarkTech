#pragma once
#include "Core.h"

template<typename T>
class CTPool
{
public:
	CTPool();
	~CTPool();

	void InitPool(size_t nInitialSize);
	void DestroyPool();

	size_t CreateElement(T value);
	void DisableElement(size_t elementIndex);

	T& GetElement(size_t elementIndex);

	size_t GetPoolSize() { return (m_nDisabled + m_nEnabled); }
	size_t GetActivatedSize() { return m_nEnabled; }
	size_t GetDeactivatedSize() { return m_nDisabled; }
private:
	T* m_pElements;
	size_t m_nDisabled;
	size_t m_nEnabled;
};

template<typename T>
inline CTPool<T>::CTPool()
	:m_pElements(nullptr), m_nDisabled(0), m_nEnabled(0)
{
}

template<typename T>
inline CTPool<T>::~CTPool()
{
}

template<typename T>
inline void CTPool<T>::InitPool(size_t nInitialSize)
{
	m_pElements = new T[nInitialSize];
	m_nDisabled = nInitialSize;
	m_nEnabled = 0;
}

template<typename T>
inline void CTPool<T>::DestroyPool()
{
	delete[] m_pElements;
}

template<typename T>
inline size_t CTPool<T>::CreateElement(T value)
{
	m_pElements[m_nEnabled] = value;
	m_nEnabled++;
	m_nDisabled--;

	return (m_nEnabled - 1);
}

template<typename T>
inline void CTPool<T>::DisableElement(size_t elementIndex)
{
	T temp = m_pElements[m_nEnabled - 1];
	m_pElements[m_nEnabled - 1] = m_pElements[elementIndex];
	m_pElements[elementIndex] = temp;
	m_nEnabled--;
	m_nDisabled++;
}

template<typename T>
inline T& CTPool<T>::GetElement(size_t elementIndex)
{
	return m_pElements[elementIndex];
}
