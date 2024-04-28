#pragma once
#include "AllocatorType.h"

template<typename T, class A = CAllocator>
class CRefCounted
{
public:
	CRefCounted(T* ptr);
	~CRefCounted();

	void AddRef();
	void RemoveRef();

	T* GetPtr() { return m_ptr; }
	uint32_t GetRefCount() { return nCount; }

private:
	uint32_t nCount;
	T* m_ptr;
	A m_Allocator;
};

template<typename T, class A>
inline CRefCounted<T,A>::CRefCounted(T* ptr)
	:m_ptr(nullptr), nCount(0)
{
	m_ptr = ptr;
}

template<typename T, class A>
inline CRefCounted<T, A>::~CRefCounted()
{
	m_ptr->~T();
	m_Allocator.Deallocate(m_ptr);
}

template<typename T, class A>
inline void CRefCounted<T, A>::AddRef()
{
	nCount++;
}

template<typename T, class A>
inline void CRefCounted<T, A>::RemoveRef()
{
	nCount--;
	if (nCount == 0)
	{
		this->~CRefCounted();
		m_Allocator.Deallocate(this);
	}
}

template<typename T, class A = CAllocator>
class CTSharedPointer
{
public:
	CTSharedPointer();
	CTSharedPointer(T* ptr);
	CTSharedPointer(const CTSharedPointer& other);
	~CTSharedPointer();

	T* GetAddress() { return m_pRef->GetPtr(); }

	void Reset();

	T* operator->();
	void operator=(const CTSharedPointer& other);
	void operator=(T* ptr);

private:
	CRefCounted<T, A>* m_pRef;
	A m_Allocator;
};

template<typename T, class A>
inline CTSharedPointer<T, A>::CTSharedPointer()
	:m_pRef(nullptr)
{
}

template<typename T, class A>
inline CTSharedPointer<T, A>::CTSharedPointer(T* ptr)
	:m_pRef(nullptr)
{
	m_pRef = (CRefCounted<T, A>*)m_Allocator.Allocate(sizeof(CRefCounted<T, A>));
	CRefCounted<T, A> temp(ptr);
	memcpy(m_pRef, &temp, sizeof(CRefCounted<T, A>));

	m_pRef->AddRef();
}

template<typename T, class A>
inline CTSharedPointer<T, A>::CTSharedPointer(const CTSharedPointer& other)
	:m_pRef(nullptr)
{
	m_pRef = other.m_pRef;
	m_pRef->AddRef();
}

template<typename T, class A>
inline CTSharedPointer<T, A>::~CTSharedPointer()
{
	if(m_pRef)
		m_pRef->RemoveRef();
}

template<typename T, class A>
inline void CTSharedPointer<T, A>::Reset()
{
	if (m_pRef)
	{
		m_pRef->RemoveRef();
		m_pRef = nullptr;
	}
}

template<typename T, class A>
inline void CTSharedPointer<T, A>::operator=(const CTSharedPointer& other)
{
	if (m_pRef)
		m_pRef->RemoveRef();

	m_pRef = other.m_pRef;
	m_pRef->AddRef();
}

template<typename T, class A>
inline void CTSharedPointer<T, A>::operator=(T* ptr)
{
	if (!m_pRef)
	{
		m_pRef = (CRefCounted<T, A>*)m_Allocator.Allocate(sizeof(CRefCounted<T, A>));
		CRefCounted<T, A> temp(ptr);
		memcpy(m_pRef, &temp, sizeof(CRefCounted<T, A>));
		m_pRef->AddRef();
	}
}

template<typename T, class A>
inline T* CTSharedPointer<T, A>::operator->()
{
	return m_pRef->GetPtr();
}
