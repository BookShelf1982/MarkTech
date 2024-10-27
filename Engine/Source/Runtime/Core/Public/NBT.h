#pragma once
#include <PrimitiveTypes.h>
#include "LinkedList.h"
#include "StackAllocator.h"

namespace MarkTech
{
	enum NBTTagType : U8
	{
		NBT_TAG_TYPE_END,
		NBT_TAG_TYPE_BYTE,
		NBT_TAG_TYPE_SHORT,
		NBT_TAG_TYPE_INT,
		NBT_TAG_TYPE_LONG,
		NBT_TAG_TYPE_FLOAT,
		NBT_TAG_TYPE_DOUBLE,
		NBT_TAG_TYPE_BYTE_ARRAY,
		NBT_TAG_TYPE_STRING,
		NBT_TAG_TYPE_LIST,
		NBT_TAG_TYPE_COMPOUND,
		NBT_TAG_TYPE_INT_ARRAY,
		NBT_TAG_TYPE_LONG_ARRAY
	};

	struct NamedTag
	{
		NBTTagType tagType;
		char* pName;
		void* pData;
	};

	struct NBTStructureCreateInfo
	{
		PoolAllocator* pPool;
		StackAllocator* pStack;
	};

	struct NBTStructure
	{
		LinkedList<NamedTag> namedTags;
		StackAllocator* pStack;
	};

	struct AddNamedTagInfo
	{
		NamedTag* pParent;
		const char* pName;
		void* pData;
	};

	struct ByteTagPayload
	{
		NBTTagType tagType;
		I8 value;
	};

	struct ShortTagPayload
	{
		NBTTagType tagType;
		I16 value;
	};

	struct IntTagPayload
	{
		NBTTagType tagType;
		I32 value;
	};

	struct LongTagPayload
	{
		NBTTagType tagType;
		I64 value;
	};

	struct StringTagPayload
	{
		NBTTagType tagType;
		char* pString;
	};

	struct CompoundTagPayload
	{
		NBTTagType tagType;
	};

	void CreateNBTStructure(NBTStructureCreateInfo* pInfo, NBTStructure* pNBT);
	NamedTag* AddNamedTag(NBTStructure* pNBT, const AddNamedTagInfo* pInfo);
	NamedTag* GetCompoundTag(const char* pName, NBTStructure* pNBT);
	void ParseNBTFile(const char* pFilepath, NBTStructure* pNBT);
	void WriteNBTToFile(const char* pFilepath, NBTStructure* pNBT);
	void DestroyNBTStructure(NBTStructure* pNBT);
}