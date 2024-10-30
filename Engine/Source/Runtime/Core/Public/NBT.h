#pragma once
#include <PrimitiveTypes.h>
#include "LinkedList.h"
#include "Array.h"

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
		MemoryAllocator* pMemAlloc;
	};

	struct NBTStructure
	{
		LinkedList<NamedTag> namedTags;
		MemoryAllocator* pMemAlloc;
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

	struct FloatTagPayload
	{
		NBTTagType tagType;
		F32 value;
	};

	struct DoubleTagPayload
	{
		NBTTagType tagType;
		F64 value;
	};

	struct ByteArrayTagPayload
	{
		NBTTagType tagType;
		U32 size;
		U8* pArray;
	};

	struct StringTagPayload
	{
		NBTTagType tagType;
		char* pString;
	};

	struct ContainerTagPayload
	{
		NBTTagType tagType;
	};

	struct IntArrayTagPayload
	{
		NBTTagType tagType;
		U32 size;
		U32* pArray;
	};

	struct LongArrayTagPayload
	{
		NBTTagType tagType;
		U32 size;
		U64* pArray;
	};

	void CreateNBTStructure(const NBTStructureCreateInfo& info, NBTStructure& nbt);
	NamedTag* AddNamedTag(NBTStructure& nbt, const AddNamedTagInfo& info);
	NamedTag* GetCompoundTag(const char* pName, NBTStructure& nbt);
	void ParseNBTFile(const char* pFilepath, NBTStructure& nbt);
	void WriteNBTToFile(const char* pFilepath, const char* pRootTagName, NBTStructure& nbt);
	void DestroyNBTStructure(NBTStructure& nbt);
}