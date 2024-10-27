#pragma once
#include <PrimitiveTypes.h>
#include "PoolAllocator.h"
#include <stdlib.h>

namespace MarkTech
{
	template<typename T>
	struct LinkedList
	{
		struct NodeType
		{
			T data;
			NodeType* pNext;
		};

		NodeType* pStart;
		NodeType* pEnd;
		PoolAllocator* pPool;
	};

	template<typename T>
	void CreateLinkedList(LinkedList<T>* pList, PoolAllocator* pPool)
	{
		pList->pStart = nullptr;
		pList->pEnd = nullptr;
		pList->pPool = pPool;
	}

	template<typename T>
	void InsertLinkedListItem(LinkedList<T>* pList, T item)
	{
		LinkedList<T>::NodeType* pNode = nullptr;
		if (pList->pPool)
			pNode = (LinkedList<T>::NodeType*)AllocFromPool(pList->pPool);
		else
			pNode = (LinkedList<T>::NodeType*)malloc(sizeof(LinkedList<T>::NodeType));

		pNode->data = item;
		pNode->pNext = nullptr;

		if (!pList->pStart)
		{
			pList->pStart = pNode;
			pList->pEnd = pNode;
		}
		else
		{
			LinkedList<T>::NodeType* pLast = pList->pStart;
			while (pLast->pNext != nullptr)
			{
				pLast = pLast->pNext;
			}

			pLast->pNext = pNode;
			pList->pEnd = pNode;
		}
	}

	template<typename T>
	void RemoveLastLinkedListItem(LinkedList<T>* pList)
	{
		if (pList->pStart == pList->pEnd)
		{
			pList->pStart = nullptr;
			pList->pEnd = nullptr;
		}
		else
		{
			LinkedList<T>::NodeType* pNode = pList->pStart;
			while (pNode->pNext->pNext != nullptr)
			{
				pNode = pNode->pNext;
			}
			pList->pEnd = pNode;
		}

		if (pList->pPool)
			FreeToPool(&pList->pPool, pList->pEnd);
		else
			free(pList->pEnd);
	}

	template<typename T>
	void DestroyLinkedList(LinkedList<T>* pList)
	{
		if (pList->pStart)
		{
			if (pList->pPool)
			{
				LinkedList<T>::NodeType* pNode = pList->pStart;
				while (pNode->pNext != nullptr)
				{
					FreeToPool(pList->pPool, pNode);
					pNode = pNode->pNext;
				}
				return;
			}

			LinkedList<T>::NodeType* pNode = pList->pStart;
			while (pNode->pNext != nullptr)
			{
				LinkedList<T>::NodeType* pNextNode = pNode->pNext;
				free(pNode);
				pNode = pNextNode;
			}
			
			free(pNode); // free last node
		}
	}
}