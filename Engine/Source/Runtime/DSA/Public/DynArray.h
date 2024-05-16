#pragma once
#include "Allocator.h"

/*
* Dynamic Array Container
* Author: Markos
*/

namespace MarkTech
{
	/*
	* Dynamic array iterator
	* Used to iterate through a dynamic array
	*/
	template<typename DynArray>
	class ArrayIterator
	{
	public:
		using ValueType = typename DynArray::ValueType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		ArrayIterator(PointerType ptr);

		ArrayIterator& operator++()
		{
			m_ptr++;
			return *this;
		}

		ArrayIterator operator++(I32)
		{
			ArrayIterator it = *this;
			++(*this);
			return it;
		}

		ArrayIterator& operator--()
		{
			m_ptr--;
			return *this;
		}

		ArrayIterator operator--(I32)
		{
			ArrayIterator it = *this;
			--(*this);
			return it;
		}

		ReferenceType operator[](U32 index)
		{
			return *(m_ptr[index]);
		}

		PointerType operator->()
		{
			return m_ptr;
		}

		ReferenceType operator*()
		{
			return *m_ptr;
		}

		bool operator==(const ArrayIterator& other) const
		{
			return m_ptr == other.m_ptr;
		}

		bool operator!=(const ArrayIterator& other) const
		{
			return !(*this == other);
		}

	private:
		PointerType m_ptr;
	};

	template<typename DynArray>
	inline ArrayIterator<DynArray>::ArrayIterator(PointerType ptr)
		:m_ptr(ptr) 
	{}

	template<typename T, class A = Allocator>
	class DynArray
	{
	public:
		using ValueType = T;
		using Iterator = ArrayIterator<DynArray<T>>;

		DynArray();
		DynArray(U32 size);
		~DynArray();

		void Insert(T obj);
		void Resize(U32 size);
		U32 Size() { return m_CurrentSize; }

		Iterator Begin() { return Iterator(m_pArray); }
		Iterator End() { return Iterator(m_pArray + m_CurrentSize); }
	private:
		T* m_pArray; // Contiguous array
		A m_Alloc; // Allocator object
		U32 m_MaxSize; // Max allocated elements
		U32 m_CurrentSize; // Current size of elements in array
	};

	/*
	* Default constructor
	* Basically set everything to zero besides the allocator
	*/
	template<typename T, class A>
	inline DynArray<T, A>::DynArray()
		:m_pArray(nullptr), m_CurrentSize(0), m_MaxSize(0)
	{
	}

	/*
	* Constructor with size arg
	* Allocates contiguous array using specified size
	*/
	template<typename T, class A>
	inline DynArray<T, A>::DynArray(U32 size)
		:m_pArray(nullptr), m_CurrentSize(0), m_MaxSize(size)
	{
		m_pArray = (T*)m_Alloc.Allocate(size * sizeof(T));
	}

	/*
	* Destructor
	* Loop through every element in array and destroy it. Then free the memory
	*/
	template<typename T, class A>
	inline DynArray<T, A>::~DynArray()
	{
		if (m_pArray)
		{
			for (U32 i = 0; i < m_CurrentSize; i++)
			{
				m_pArray[i].~T();
			}
			m_Alloc.Free(m_pArray);
		}
	}

	/*
	* Insert Func
	* Insert an element into the array
	*/
	template<typename T, class A>
	inline void DynArray<T, A>::Insert(T obj)
	{
		// Is the current size of elements in array equal to the allocated size?
		if (m_CurrentSize == m_MaxSize)
		{
			U32 oldSize = m_CurrentSize;

			// Allocate larger array
			T* pTemp = m_pArray;
			m_pArray = (T*)m_Alloc.Allocate((m_CurrentSize + 1) * sizeof(T));

			// Copy previous array to new array
			for (U32 i = 0; i < m_CurrentSize; i++)
			{
				m_pArray[i] = pTemp[i];
			}

			// Free old array
			for (U32 i = 0; i < oldSize; i++)
			{
				m_pArray[i].~T();
			}
			m_Alloc.Free(m_pArray);
		}

		// Put object into array and increment current size
		m_pArray[m_CurrentSize] = obj;
		m_CurrentSize++;
	}

	/*
	* Resize Func
	* Resizes the array
	*/
	template<typename T, class A>
	inline void DynArray<T, A>::Resize(U32 size)
	{
	}
}