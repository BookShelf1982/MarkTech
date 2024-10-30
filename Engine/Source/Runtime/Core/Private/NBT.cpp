#include "NBT.h"
#include <File.h>
#include <Log.h>
#include <Byteswap.h>
#include <zlib.h>

namespace MarkTech
{
	static void ReadTags(gzFile file, NBTStructure& nbt)
	{
		LinkedList<NamedTag*> parentTags;
		CreateLinkedList(parentTags, nullptr);
		while (true)
		{
			AddNamedTagInfo info;
			NBTTagType type = NBT_TAG_TYPE_END;
			I16 nameLength = 0;
			char name[256] = "";
			gzread(file, &type, 1);
			
			info.pParent = parentTags.pEnd ? parentTags.pEnd->data : nullptr;
			if (type == NBT_TAG_TYPE_END)
			{
				if (info.pParent)
				{
					RemoveLastLinkedListItem(parentTags);
					continue;
				}
				else
				{
					DestroyLinkedList(parentTags);
					return;
				}
			}

			gzread(file, &nameLength, 2);
			nameLength = SwapI16(nameLength);
			gzread(file, name, nameLength);
			info.pName = name;

			switch (type)
			{
			case NBT_TAG_TYPE_BYTE:
			{
				ByteTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_BYTE;
				gzread(file, &payload.value, 1);
				info.pData = &payload;
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_SHORT:
			{
				ShortTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_SHORT;
				gzread(file, &payload.value, 2);
				payload.value = SwapI16(payload.value);
				info.pData = &payload;
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_INT:
			{
				IntTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_INT;
				gzread(file, &payload.value, 4);
				payload.value = SwapI32(payload.value);
				info.pData = &payload; 
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_LONG:
			{
				LongTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_INT;
				gzread(file, &payload.value, 8);
				payload.value = SwapI64(payload.value);
				info.pData = &payload;
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_FLOAT:
			{
				FloatTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_FLOAT;
				gzread(file, &payload.value, 4);
				I32 value = SwapI32(*(I32*)&payload.value);
				payload.value = *(F32*)&value;
				info.pData = &payload;
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_DOUBLE:
			{
				DoubleTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_DOUBLE;
				gzread(file, &payload.value, 8);
				I64 value = SwapI64(*(I64*)&payload.value);
				payload.value = *(F64*)&value;
				info.pData = &payload;
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_BYTE_ARRAY:
				break;
			case NBT_TAG_TYPE_STRING:
			{
				StringTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_STRING;
				char string[256] = "";
				payload.pString = string;
				I16 strLen = 0;
				gzread(file, &strLen, 2);
				strLen = SwapI16(strLen);
				gzread(file, string, strLen);
				info.pData = &payload;
				AddNamedTag(nbt, info);
			} break;
			case NBT_TAG_TYPE_LIST:
				break;
			case NBT_TAG_TYPE_COMPOUND:
			{
				ContainerTagPayload payload;
				payload.tagType = NBT_TAG_TYPE_COMPOUND;
				info.pData = &payload;
				NamedTag* pTag = AddNamedTag(nbt, info);
				InsertLinkedListItem(parentTags, pTag);
			} break;
			case NBT_TAG_TYPE_INT_ARRAY:
				break;
			case NBT_TAG_TYPE_LONG_ARRAY:
				break;
			}
		}
	}

	static void WriteTags(gzFile file, LinkedList<NamedTag>& tagList)
	{
		I8 null = 0;
		LinkedList<NamedTag>::NodeType* pTag = tagList.pStart;
		while (pTag != nullptr )
		{
			I8 type = pTag->data.tagType;
			gzwrite(file, &type, 1);
			
			I16 nameLength = (I16)strlen(pTag->data.pName);
			I16 nameLengthSwapped = SwapI16(nameLength);
			gzwrite(file, &nameLengthSwapped, 2);
			gzwrite(file, pTag->data.pName, nameLength);
			
			switch (type)
			{
			case NBT_TAG_TYPE_BYTE:
			{
				gzwrite(file, pTag->data.pData, 1);
			} break;
			case NBT_TAG_TYPE_SHORT:
			{
				I16 num = *(I16*)pTag->data.pData;
				num = SwapI16(num);
				gzwrite(file, &num, 2);
			} break;
			case NBT_TAG_TYPE_INT:
			{
				I32 num = *(I32*)pTag->data.pData;
				num = SwapI32(num);
				gzwrite(file, &num, 4);
			} break;
			case NBT_TAG_TYPE_LONG:
			{
				I64 num = *(I64*)pTag->data.pData;
				num = SwapI64(num);
				gzwrite(file, &num, 8);
			} break;
			case NBT_TAG_TYPE_FLOAT:
			{
				I32 num = *(I32*)pTag->data.pData;
				num = SwapI32(num);
				gzwrite(file, &num, 4);
			} break;
			case NBT_TAG_TYPE_DOUBLE:
			{
				I64 num = *(I64*)pTag->data.pData;
				num = SwapI64(num);
				gzwrite(file, &num, 8);
			} break;
			case NBT_TAG_TYPE_STRING:
			{
				char* pStr = (char*)pTag->data.pData;
				I16 length = (I16)strlen(pStr);
				I16 lengthSwapped = SwapI16(length);
				gzwrite(file, &lengthSwapped, 2);
				gzwrite(file, pStr, length);
			} break;
			case NBT_TAG_TYPE_COMPOUND:
			{
				LinkedList<NamedTag>* pTagList = (LinkedList<NamedTag>*)pTag->data.pData;
				WriteTags(file, tagList);
			} break;
			case NBT_TAG_TYPE_BYTE_ARRAY:
			{
				Array<U8>* pArray = (Array<U8>*)pTag->data.pData;
				I32 arrLen = SwapI32(pArray->size);
				gzwrite(file, &arrLen, 4);
				gzwrite(file, pArray->pArray, sizeof(U8) * pArray->size);
			} break;
			}

			pTag = pTag->pNext;
		}

		gzwrite(file, &null, 1);
	}

	static void DestroyCompundTagChildren(LinkedList<NamedTag>& children)
	{
		LinkedList<NamedTag>::NodeType* pTag = children.pStart;
		while (pTag != nullptr)
		{
			if (pTag->data.tagType == NBT_TAG_TYPE_COMPOUND)
			{
				DestroyCompundTagChildren((LinkedList<NamedTag>&)*(LinkedList<NamedTag>*)pTag->data.pData);
			}

			pTag = pTag->pNext;
		}

		DestroyLinkedList(children);
	}

	void CreateNBTStructure(const NBTStructureCreateInfo& info, NBTStructure& nbt)
	{
		CreateLinkedList(nbt.namedTags, info.pPool);
		nbt.pMemAlloc = info.pMemAlloc;
	}

	NamedTag* AddNamedTag(NBTStructure& nbt, const AddNamedTagInfo& info)
	{
		U64 length = strlen(info.pName);
		char* pNameCpy = (char*)AllocFromMemAllocator(*nbt.pMemAlloc, (U32)(length + 1));
		strcpy_s(pNameCpy, length + 1, info.pName);
		pNameCpy[length] = 0;

		NamedTag tag;
		NBTTagType type = *(NBTTagType*)info.pData;
		tag.tagType = type;
		tag.pName = pNameCpy;

		switch (type)
		{
		case NBT_TAG_TYPE_BYTE:
		{
			I8* pData = (I8*)AllocFromMemAllocator(*nbt.pMemAlloc, 1);
			ByteTagPayload* data = (ByteTagPayload*)info.pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_SHORT:
		{
			I16* pData = (I16*)AllocFromMemAllocator(*nbt.pMemAlloc, 2);
			ShortTagPayload* data = (ShortTagPayload*)info.pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_INT:
		{
			I32* pData = (I32*)AllocFromMemAllocator(*nbt.pMemAlloc, 4);
			IntTagPayload* data = (IntTagPayload*)info.pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_LONG:
		{
			I64* pData = (I64*)AllocFromMemAllocator(*nbt.pMemAlloc, 8);
			LongTagPayload* data = (LongTagPayload*)info.pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_FLOAT:
		{
			F32* pData = (F32*)AllocFromMemAllocator(*nbt.pMemAlloc, 4);
			FloatTagPayload* data = (FloatTagPayload*)info.pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_DOUBLE:
		{
			F64* pData = (F64*)AllocFromMemAllocator(*nbt.pMemAlloc, 8);
			DoubleTagPayload* data = (DoubleTagPayload*)info.pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_BYTE_ARRAY:
		{
			ByteArrayTagPayload* pPayload = (ByteArrayTagPayload*)info.pData;
			Array<U8>* pByteArray = (Array<U8>*)AllocFromMemAllocator(*nbt.pMemAlloc, sizeof(Array<U8>));
			pByteArray->pArray = nullptr;
			pByteArray->reservedSize = pPayload->size;
			pByteArray->size = pPayload->size;
			ReserveArray(*pByteArray, pPayload->size, nbt.pMemAlloc);
			for (U32 i = 0; i < pByteArray->size; i++)
			{
				pByteArray->pArray[i] = pPayload->pArray[i];
			}
			tag.pData = pByteArray;
		} break;
		case NBT_TAG_TYPE_STRING:
		{
			StringTagPayload* pPayload = (StringTagPayload*)info.pData;
			U64 len = strlen(pPayload->pString);
			char* pStr = (char*)AllocFromMemAllocator(*nbt.pMemAlloc, (U32)(len + 1));
			strcpy_s(pStr, len + 1, pPayload->pString);
			pStr[len] = 0;
			tag.pData = pStr;
		} break;
		case NBT_TAG_TYPE_COMPOUND:
		{
			LinkedList<NamedTag>* pList = (LinkedList<NamedTag>*)AllocFromMemAllocator(*nbt.pMemAlloc, sizeof(LinkedList<NamedTag>));
			CreateLinkedList(*pList, nbt.namedTags.pPool);
			tag.pData = pList;
		} break;
		}

		if (info.pParent)
		{
			if (info.pParent->tagType == NBT_TAG_TYPE_COMPOUND)
			{
				LinkedList<NamedTag>* pTagEntries = (LinkedList<NamedTag>*)info.pParent->pData;
				InsertLinkedListItem(*pTagEntries, tag);
				return &pTagEntries->pEnd->data;
			}
		}

		InsertLinkedListItem(nbt.namedTags, tag);
		return &nbt.namedTags.pEnd->data;
	}

	NamedTag* GetCompoundTag(const char* pName, NBTStructure& nbt)
	{
		LinkedList<NamedTag>::NodeType* pTag = nbt.namedTags.pStart;
		while (pTag != nullptr)
		{
			if (pTag->data.tagType == NBT_TAG_TYPE_COMPOUND)
			{
				if (strcmp(pTag->data.pName, pName) == 0)
				{
					return &pTag->data;
				}
			}

			pTag = pTag->pNext;
		}

		return nullptr;
	}

	void ParseNBTFile(const char* pFilepath, NBTStructure& nbt)
	{
		gzFile file = gzopen(pFilepath, "rb");
		I8 type = 0;
		gzread(file, &type, 1);
		I16 length = 0;
		gzread(file, &length, 2);
		length = SwapI16(length);
		char name[256] = "";
		gzread(file, name, length);

		ReadTags(file, nbt);
		gzclose(file);
	}

	void WriteNBTToFile(const char* pFilepath, const char* pRootTagName, NBTStructure& nbt)
	{
		gzFile file = gzopen(pFilepath, "wb");

		const NBTTagType type = NBT_TAG_TYPE_COMPOUND;
		gzwrite(file, &type, 1);
		if (pRootTagName)
		{
			I16 namelen = (I16)strlen(pRootTagName);
			I16 namelenSwapped = SwapI16(namelen);

			gzwrite(file, &namelenSwapped, 2);
			gzwrite(file, pRootTagName, namelen);
		}
		else
		{
			I16 zero = 0;
			gzwrite(file, &zero, 2);
		}

		WriteTags(file, nbt.namedTags);
		gzclose(file);
	}

	void DestroyNBTStructure(NBTStructure& nbt)
	{
		DestroyCompundTagChildren(nbt.namedTags);
	}
}