#pragma once
#include "alloc_uitls.h"
#include <stdint.h>

inline uint32_t Hash(uint32_t x)
{
	return x;
}

#define FIXED_ARRAY_SIZE(a) sizeof(a) / sizeof(a[0])

/*
* Optional
*/
template<typename T>
struct Optional
{
	Optional() : null(true), retVal(T()) {}
	Optional(T& other) : null(false), retVal(other) {}

	inline operator T() const { return retVal; }
	inline bool operator!() { return null; }
	
	T& retVal;
	bool null;
};

/*
* Array Class
*/

template <typename T>
struct Array
{
	inline T& operator[](size_t index) { return elements[index]; };

	T* elements;
	size_t capacity;
	size_t size;
};

/*
* Fixed Array Wrapper
*/

template <typename T, size_t S>
struct FixedArray
{
	inline T& operator[](size_t index) { return elements[index]; }

	T elements[S];
	size_t count;
};

/*
* Hash Table
*/

template<typename Key, typename Val>
class HashTable
{
public:
	struct HashTableNode
	{
		Key key;
		Val value;
		HashTableNode* next;
	};

	HashTable() = default;
	HashTable(size_t tableSize);
	~HashTable();
	
	inline size_t GetTableSize() const { return m_tableSize; }
	inline Optional<Val> operator[](Key key) const { return GetVal(key); }
	Optional<Val> GetVal(Key key) const;

	void Insert(Key key, Val value);
	void Remove(Key key);

private:
	Array<HashTableNode*> m_table;
	size_t m_capacity;
	size_t m_size;
};

template<typename Key, typename Val>
inline HashTable<Key, Val>::HashTable(size_t tableSize)
	:m_table(tableSize), m_size(0), m_capacity(tableSize)
{
	for (size_t i = 0; i < tableSize; i++)
		m_table[i] = nullptr;
}

template<typename Key, typename Val>
inline HashTable<Key, Val>::~HashTable()
{
	for (size_t i = 0; i < m_capacity; i++)
	{
		HashTableNode* node = m_table[i];
		while (node != nullptr)
		{
			node->~HashTableNode();
			HashTableNode* n = node;
			node = node->next;
			AlignedFree(n);
		}
	}
}

template<typename Key, typename Val>
inline Optional<Val> HashTable<Key, Val>::GetVal(Key key) const
{
	return Optional<Val>();
}

template<typename Key, typename Val>
inline void HashTable<Key, Val>::Insert(Key key, Val value)
{
	size_t hash = Hash(key);
	size_t index = hash % m_capacity;

	HashTableNode* node = m_table[index];
	HashTableNode* prevNode = nullptr;
	while (node != nullptr)
	{
		node = node->next;
		if (!node->next)
			prevNode = node;
	}

	HashTableNode* newNode = (HashTableNode*)AlignedAlloc(sizeof(HashTableNode), alignof(HashTableNode));
	newNode->key = key;
	newNode->next = nullptr;
	newNode->value = value;
	node->next = newNode;
	prevNode->next = node;
}

template<typename Key, typename Val>
inline void HashTable<Key, Val>::Remove(Key key)
{
	size_t hash = Hash(key);
	size_t index = hash % m_capacity;
}

template <typename Key, typename Val, size_t S>
struct FixedHashMap
{
	struct Node
	{
		Key key;
		Val value;
	};

	Node map[S];
	size_t count;
};