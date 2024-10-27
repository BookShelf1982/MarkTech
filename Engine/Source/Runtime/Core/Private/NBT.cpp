#include "NBT.h"
#include <File.h>
#include <Log.h>
#include <Byteswap.h>
#include <zlib.h>

namespace MarkTech
{
	static NamedTag* ReadTag(gzFile file, NBTStructure* pNBT)
	{
		NBTTagType type = NBT_TAG_TYPE_END;
		I16 nameLength = 0;
		gzread(file, &type, sizeof(NBTTagType));
		if (type == NBT_TAG_TYPE_END)
			return nullptr;

		gzread(file, &nameLength, sizeof(I16));
		nameLength = SwapI16(nameLength);
		char* pName = (char*)AllocFromStack(pNBT->pStack, nameLength + 1);
		gzread(file, pName, nameLength);
		pName[nameLength] = 0;

		NamedTag tag;
		tag.tagType = type;
		tag.pName = pName;
		
		/*switch (type)
		{
		case NBT_TAG_TYPE_END:
			break;
		case NBT_TAG_TYPE_BYTE:
			tag.pData = AllocFromStack(pNBT->pStack, 1);
			gzread(file, tag.pData, 1);
			break;
		case NBT_TAG_TYPE_SHORT:
			tag.pData = AllocFromStack(pNBT->pStack, 2);
			gzread(file, tag.pData, 2);
			*(I16*)tag.pData = SwapI16(*(I16*)tag.pData);
			break;
		case NBT_TAG_TYPE_INT:
			tag.pData = AllocFromStack(pNBT->pStack, 4);
			gzread(file, tag.pData, 4);
			*(I32*)tag.pData = SwapI32(*(I32*)tag.pData);
			break;
		case NBT_TAG_TYPE_LONG:
			tag.pData = AllocFromStack(pNBT->pStack, 8);
			gzread(file, tag.pData, 8);
			*(I64*)tag.pData = SwapI64(*(I64*)tag.pData);
			break;
		case NBT_TAG_TYPE_FLOAT:
			tag.pData = AllocFromStack(pNBT->pStack, 4);
			gzread(file, tag.pData, 4);
			*(I32*)tag.pData = SwapI32(*(I32*)tag.pData);
			break;
		case NBT_TAG_TYPE_DOUBLE:
			tag.pData = AllocFromStack(pNBT->pStack, 8);
			gzread(file, tag.pData, 8);
			*(I64*)tag.pData = SwapI64(*(I64*)tag.pData);
			break;
		case NBT_TAG_TYPE_BYTE_ARRAY:
		{
			I32 length = 0;
			gzread(file, &length, 4);
			length = SwapI32(length);

			tag.pData = AllocFromStack(pNBT->pStack, length + 4);
			char* pCursor = (char*)tag.pData;
			*(I32*)pCursor = length;
			pCursor += 4;

			gzread(file, pCursor, length);
		} break;
		case NBT_TAG_TYPE_STRING:
		{
			I16 length = 0;
			gzread(file, &length, 2);
			length = SwapI16(length);

			tag.pData = AllocFromStack(pNBT->pStack, length + 2);
			char* pCursor = (char*)tag.pData;
			*(I32*)pCursor = length;
			pCursor += 2;

			gzread(file, pCursor, length);
		} break;
		case NBT_TAG_TYPE_LIST:
		{

		} break;
		case NBT_TAG_TYPE_COMPOUND:
			InsertLinkedListItem<NamedTag>(&pNBT->namedTags, tag);
			pNBT->namedTags.pStart->data.pData = ReadTag(file, pNBT);
			return &pNBT->namedTags.pStart->data;
		case NBT_TAG_TYPE_INT_ARRAY:
			break;
		case NBT_TAG_TYPE_LONG_ARRAY:
			break;
		}
		
		InsertLinkedListItem<NamedTag>(&pNBT->namedTags, tag);

		LinkedList<NamedTag>::NodeType* pNode = pNBT->namedTags.pStart;

		while (pNode->pNext != nullptr)
		{
			pNode = pNode->pNext;
		}

		return &pNode->data;*/
		return nullptr;
	}

	static void WriteTags(gzFile file, LinkedList<NamedTag>* pTagList)
	{
		I8 null = 0;
		LinkedList<NamedTag>::NodeType* pTag = pTagList->pStart;
		while (pTag != nullptr )
		{
			I8 type = pTag->data.tagType;
			gzwrite(file, &type, 1);
			
			I16 nameLength = strlen(pTag->data.pName);
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
				WriteTags(file, pTagList);
			} break;
			}

			pTag = pTag->pNext;
		}

		gzwrite(file, &null, 1);
	}

	static void DestroyCompundTagChildren(LinkedList<NamedTag>* pChildren)
	{
		LinkedList<NamedTag>::NodeType* pTag = pChildren->pStart;
		while (pTag != nullptr)
		{
			if (pTag->data.tagType == NBT_TAG_TYPE_COMPOUND)
			{
				DestroyCompundTagChildren((LinkedList<NamedTag>*)pTag->data.pData);
			}

			pTag = pTag->pNext;
		}

		DestroyLinkedList(pChildren);
	}

	void CreateNBTStructure(NBTStructureCreateInfo* pInfo, NBTStructure* pNBT)
	{
		CreateLinkedList(&pNBT->namedTags, pInfo->pPool);
		pNBT->pStack = pInfo->pStack;
	}

	NamedTag* AddNamedTag(NBTStructure* pNBT, const AddNamedTagInfo* pInfo)
	{
		U64 length = strlen(pInfo->pName);
		char* pNameCpy = (char*)AllocFromStack(pNBT->pStack, length + 1);
		strcpy_s(pNameCpy, length + 1, pInfo->pName);
		pNameCpy[length] = 0;

		NamedTag tag;
		NBTTagType type = *(NBTTagType*)pInfo->pData;
		tag.tagType = type;
		tag.pName = pNameCpy;
		
		switch (type)
		{
		case NBT_TAG_TYPE_BYTE:
		{
			I8* pData = (I8*)AllocFromStack(pNBT->pStack, 1);
			ByteTagPayload* data = (ByteTagPayload*)pInfo->pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_SHORT:
		{
			I16* pData = (I16*)AllocFromStack(pNBT->pStack, 2);
			ShortTagPayload* data = (ShortTagPayload*)pInfo->pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_INT:
		{
			I32* pData = (I32*)AllocFromStack(pNBT->pStack, 4);
			IntTagPayload* data = (IntTagPayload*)pInfo->pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_LONG:
		{
			I64* pData = (I64*)AllocFromStack(pNBT->pStack, 8);
			LongTagPayload* data = (LongTagPayload*)pInfo->pData;
			*pData = data->value;
			tag.pData = pData;
		} break;
		case NBT_TAG_TYPE_STRING:
		{
			StringTagPayload* pPayload = (StringTagPayload*)pInfo->pData;
			U64 len = strlen(pPayload->pString);
			char* pStr = (char*)AllocFromStack(pNBT->pStack, len + 1);
			strcpy_s(pStr, len + 1, pPayload->pString);
			pStr[len] = 0;
			tag.pData = pStr;
		} break;
		case NBT_TAG_TYPE_COMPOUND:
		{
			LinkedList<NamedTag>* pList = (LinkedList<NamedTag>*)AllocFromStack(pNBT->pStack, sizeof(LinkedList<NamedTag>));
			CreateLinkedList(pList, pNBT->namedTags.pPool);
			tag.pData = pList;
		} break;
		}

		if (pInfo->pParent)
		{
			if (pInfo->pParent->tagType == NBT_TAG_TYPE_COMPOUND)
			{
				LinkedList<NamedTag>* pTagEntries = (LinkedList<NamedTag>*)pInfo->pParent->pData;
				InsertLinkedListItem(pTagEntries, tag);
				return &pTagEntries->pEnd->data;
			}
		}

		InsertLinkedListItem(&pNBT->namedTags, tag);
		return &pNBT->namedTags.pEnd->data;
	}

	NamedTag* GetCompoundTag(const char* pName, NBTStructure* pNBT)
	{
		LinkedList<NamedTag>::NodeType* pTag = pNBT->namedTags.pStart;
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

	void ParseNBTFile(const char* pFilepath, NBTStructure* pNBT)
	{
		gzFile file = gzopen(pFilepath, "rb");
		
		NamedTag* pLastTag = nullptr;
		do
		{
			pLastTag = ReadTag(file, pNBT);
		} while (pLastTag != nullptr);

		gzclose(file);
	}

	void WriteNBTToFile(const char* pFilepath, NBTStructure* pNBT)
	{
		gzFile file = gzopen(pFilepath, "wb");

		const NBTTagType type = NBT_TAG_TYPE_COMPOUND;
		const char name[] = "root";
		I16 namelen = SwapI16(sizeof(name) - 1);

		gzwrite(file, &type, 1);
		gzwrite(file, &namelen, 2);
		gzwrite(file, name, sizeof(name) - 1);

		WriteTags(file, &pNBT->namedTags);

		gzclose(file);
	}

	void DestroyNBTStructure(NBTStructure* pNBT)
	{
		DestroyCompundTagChildren(&pNBT->namedTags);
		FreeStackAllocator(pNBT->pStack);
	}
}