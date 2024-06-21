#pragma once
#include "PrimitiveTypes.h"
#include "DSManager.h"
#include <memory.h>

/*
* Linked List container
* Author: Markos
*/

namespace MarkTech
{
	/*
	* Node type in the linked list.
	*/
	template<typename T>
	struct LinkedNode
	{
		LinkedNode();
		~LinkedNode();

		LinkedNode<T>* pNextNode;
		T data;
	};

	template<typename T>
	inline LinkedNode<T>::LinkedNode()
		:pNextNode(nullptr), data()
	{
	}


	template<typename T>
	inline LinkedNode<T>::~LinkedNode()
	{
	}

	/*
	* List Iterator iterates through a linked list
	*/
	template<typename LinkedList>
	class ListIterator
	{
	public:
		using ValueType = typename LinkedList::NodeType;
		using PointerType = ValueType*;
		using ReferenceType = ValueType&;

		ListIterator(PointerType node)
			:m_ptr(node)
		{
		}

		void operator++(I32)
		{
			m_ptr = m_ptr->pNextNode;
		}

		bool operator==(const ListIterator& other)
		{
			return m_ptr == other.m_ptr;
		}

		bool operator!=(const ListIterator& other)
		{
			return m_ptr != other.m_ptr;
		}

		PointerType operator->()
		{
			return m_ptr;
		}
	private:
		PointerType m_ptr;
	};

	/*
	* Linked list container
	*/
	template<typename T>
	class LinkedList
	{
	public:
		LinkedList();
		~LinkedList();
		using Iterator = ListIterator<LinkedList<T>>;
		using NodeType = LinkedNode<T>;

		void Insert(const T& element);
		void Remove(U32 index);
		Iterator Begin() { return Iterator(m_pFirstNode); }
	private:
		LinkedNode<T>* m_pFirstNode;
		U32 m_Size;
		U32 m_PreallocatedNodeCount;
	};

	/*
	* Sets values to 0
	*/
	template<typename T>
	inline LinkedList<T>::LinkedList()
		:m_pFirstNode(nullptr), m_Size(0), m_PreallocatedNodeCount(0)
	{
	}

	/*
	* Destroys all nodes and frees node memory
	*/
	template<typename T>
	inline LinkedList<T>::~LinkedList()
	{
		if (!m_pFirstNode)
			return;

		LinkedNode<T>* pCurrentNode = m_pFirstNode;
		LinkedNode<T>* pNextNode = m_pFirstNode->pNextNode;

		while (pCurrentNode != nullptr)
		{
			pNextNode = pCurrentNode->pNextNode;
			pCurrentNode->~LinkedNode();
			DSManager::FreeBlock(pCurrentNode, sizeof(LinkedNode<T>));
			pCurrentNode = pNextNode;
		}

		m_pFirstNode = nullptr;
	}

	/*
	* Insert a node in linked list
	*/
	template<typename T>
	inline void LinkedList<T>::Insert(const T& element)
	{
		LinkedNode<T>* pNode = m_pFirstNode;
		LinkedNode<T>* pPrevNode = nullptr;
		
		while (pNode != nullptr)
		{
			pPrevNode = pNode;
			pNode = pNode->pNextNode;
		}

		pNode = (LinkedNode<T>*)DSManager::AllocBlock(sizeof(LinkedNode<T>));
		LinkedNode<T> temp;
		temp.data = element;
		memcpy(pNode, &temp, sizeof(temp));

		if (!m_pFirstNode)
			m_pFirstNode = pNode;
		else
			pPrevNode->pNextNode = pNode;

		m_Size++;
	}

	/*
	* Removes a node from the linked list
	*/
	template<typename T>
	inline void LinkedList<T>::Remove(U32 index)
	{
		LinkedNode<T>* pNode = m_pFirstNode;
		LinkedNode<T>* pPrevNode = nullptr;

		for (U32 i = 0; i < index; i++)
		{
			pPrevNode = pNode;
			pNode = pNode->pNextNode;
		}
		
		if (!pNode)
			return;

		pPrevNode->pNextNode = pNode->pNextNode;
		
		pNode->~LinkedNode();
		DSManager::FreeBlock(pNode, sizeof(LinkedNode<T>));
		m_Size--;
	}
}