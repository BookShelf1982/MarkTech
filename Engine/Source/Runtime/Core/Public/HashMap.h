#pragma once
#include <PrimitiveTypes.h>
#include <Array.h>
#include <LinkedList.h>

#include <string.h>

namespace MarkTech
{
	template<typename Key, typename Val>
	struct HashMap
	{
		struct KeyValType
		{
			Key key;
			Val value;
		};

		Array<LinkedList<KeyValType>> map;
	};

	struct AllocatorInfo
	{
		PoolAllocator* pPoolAlloc;
		MemoryAllocator* pMemAlloc;
	};

	U32 HashKey(U32 key, U32 listSize);
	bool CompareKey(U32 src, U32 key);
	U32 HashKey(const char* pKey, U32 listSize);
	bool CompareKey(const char* pSrc, const char* pKey);

	template<typename Key, typename Val>
	void CreateHashMap(HashMap<Key, Val>& map, U32 listSize, const AllocatorInfo* pAllocInfo)
	{
		MemoryAllocator* pMemAlloc = pAllocInfo ? pAllocInfo->pMemAlloc : nullptr;
		PoolAllocator* pPoolAlloc = pAllocInfo ? pAllocInfo->pPoolAlloc : nullptr;

		ReserveArray(map.map, listSize, pMemAlloc);
		for (U32 i = 0; i < map.map.reservedSize; i++)
		{
			CreateLinkedList(map.map.pArray[i], pPoolAlloc);
		}
	}

	template<typename Key, typename Val>
	void DestroyHashMap(HashMap<Key, Val>& map)
	{
		for (U32 i = 0; i < map.map.reservedSize; i++)
		{
			DestroyLinkedList(map.map.pArray[i]);
		}

		DestroyArray(map.map);
	}

	template<typename Key, typename Val>
	void InsertHashMapItem(HashMap<Key, Val>& map, Key key, Val value)
	{
		U32 idx = HashKey(key, map.map.reservedSize);
		HashMap<Key, Val>::KeyValType item = { key, value };
		InsertLinkedListItem(map.map.pArray[idx], item);
	}

	template<typename Key, typename Val>
	void RemoveHashMapItem(HashMap<Key, Val>& map, Key key)
	{
		U32 idx = HashKey(key, map.map.reservedSize);
		LinkedList<HashMap<Key, Val>::KeyValType>::NodeType* pNode = map.map.pArray[idx].pStart;
		while (pNode != nullptr)
		{
			if (CompareKey(key, pNode->data.key))
			{
				RemoveLinkedListItem(map.map.pArray[idx], pNode);
				break;
			}

			pNode = pNode->pNext;
		}
	}

	template<typename Key, typename Val>
	bool GetHashMapItem(HashMap<Key, Val>& map, Key key, Val* pVal)
	{
		U32 idx = HashKey(key, map.map.reservedSize);
		LinkedList<HashMap<Key, Val>::KeyValType>::NodeType* pNode = map.map.pArray[idx].pStart;
		while (pNode != nullptr)
		{
			if (CompareKey(key, pNode->data.key))
			{
				*pVal = pNode->data.value;
				return true;
			}

			pNode = pNode->pNext;
		}

		return false;
	}
}