#include "nbt.h"
#include <zlib.h>

#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define BYTE_SWAP16(x) _byteswap_ushort(x)
#define BYTE_SWAP32(x) _byteswap_ulong(x)
#define BYTE_SWAP64(x) _byteswap_uint64(x)
#else
#define BYTE_SWAP16(x) x
#define BYTE_SWAP32(x) x
#define BYTE_SWAP64(x) x
#endif

nbt_malloc_func pfn_malloc = malloc;
nbt_free_func pfn_free = free;

size_t allocSizeTable[] = { 0, 1, 2, 4, 8, 4, 8, 12, 10, 16, 12, 12, 12 };

void nbt_init(const nbt_init_info_t* info) {
	if (info) {
		if (info->malloc) {
			pfn_malloc = info->malloc;
		}
		if (info->free) {
			pfn_free = info->free;
		}
	}
}

void nbt_shutdown() {}

void nbt_create_tag(const nbt_create_tag_info_t* info, nbt_tag* pTag) {
	// alloc tag
	nbt_tag tag = (nbt_tag)pfn_malloc(sizeof(nbt_tag_t));
	*pTag = tag;
	tag->type = info->type;

	// alloc name
	size_t nameLen = strlen(info->name);
	tag->name = pfn_malloc(nameLen + 1);
	strcpy_s(tag->name, nameLen + 1, info->name);

	// alloc payload
	tag->payload = (nbt_tag_payload_t*)pfn_malloc(allocSizeTable[info->type]);
	memcpy_s(tag->payload, allocSizeTable[info->type], &info->payload, allocSizeTable[info->type]);
}

void nbt_destroy_tag(nbt_tag tag) {
	if (tag->type == NBT_TAG_TYPE_COMPOUND) {
		pfn_free(tag->payload->compoundTag.tags); // free tags array
	}
	if (tag->type == NBT_TAG_TYPE_LIST) {
		pfn_free(tag->payload->listTag.list);
	}

	// free payload
	pfn_free(tag->payload);
	// free name
	pfn_free(tag->name);
	// free tag
	pfn_free(tag);
}

void nbt_compound_add_tag(nbt_tag compoundTag, nbt_tag tag) {
	if (compoundTag->type != NBT_TAG_TYPE_COMPOUND) {
		return;
	}

	if (compoundTag->payload->compoundTag.tags) {
		size_t prevAllocSize = compoundTag->payload->compoundTag.size * 8;
		size_t allocSize = (compoundTag->payload->compoundTag.size + 1) * 8;
		nbt_tag_t** ppTags = (nbt_tag_t**)pfn_malloc(allocSize); // realloc tags array
		memcpy_s(ppTags, allocSize, compoundTag->payload->compoundTag.tags, prevAllocSize); // copy previous array into new array
		pfn_free(compoundTag->payload->compoundTag.tags);
		compoundTag->payload->compoundTag.tags = ppTags;
	}
	else {
		compoundTag->payload->compoundTag.tags = (nbt_tag_t**)pfn_malloc(8);
	}

	compoundTag->payload->compoundTag.tags[compoundTag->payload->compoundTag.size] = tag;
	compoundTag->payload->compoundTag.size++; // increment size
}

void nbt_compound_remove_tag(nbt_tag compoundTag, char* name) {
	if (compoundTag->type != NBT_TAG_TYPE_COMPOUND) {
		return;
	}

	// find item by name
	int32_t indexToSkip = -1;
	for (int32_t i = 0; i < compoundTag->payload->compoundTag.size; i++) {
		if (strcmp(compoundTag->payload->compoundTag.tags[i]->name, name) == 0) {
			indexToSkip = i;
			break;
		}
	}

	// if the item hasn't been found then stop
	if (indexToSkip == -1) {
		return;
	}

	// realloc array
	size_t allocSize = (compoundTag->payload->compoundTag.size - 1) * 8;
	nbt_tag_t** ppTags = (nbt_tag_t**)pfn_malloc(allocSize);
	for (int32_t i = 0; i < compoundTag->payload->compoundTag.size; i++) {
		if (i == indexToSkip) {
			continue;
		}

		ppTags[i] = compoundTag->payload->compoundTag.tags[i];
	}

	pfn_free(compoundTag->payload->compoundTag.tags);
	compoundTag->payload->compoundTag.tags = ppTags;
	compoundTag->payload->compoundTag.size--;
}

void nbt_compound_get_tag(nbt_tag compoundTag, char* name, nbt_tag* pTag) {
	// find item by name
	for (int32_t i = 0; i < compoundTag->payload->compoundTag.size; i++) {
		if (strcmp(compoundTag->payload->compoundTag.tags[i]->name, name) == 0) {
			*pTag = compoundTag->payload->compoundTag.tags[i];
			return;
		}
	}
}

void nbt_list_append_tag(nbt_tag listTag, nbt_tag_payload_t payload) {
	if (listTag->type != NBT_TAG_TYPE_LIST) {
		return;
	}

	if (listTag->payload->listTag.list) {
		size_t prevAllocSize = listTag->payload->listTag.listSize * allocSizeTable[listTag->payload->listTag.listType];
		size_t allocSize = (listTag->payload->listTag.listSize + 1) * allocSizeTable[listTag->payload->listTag.listType];
		nbt_tag_payload_t* pPayloads = (nbt_tag_payload_t*)pfn_malloc(allocSize); // realloc tags array
		memcpy_s(pPayloads, allocSize, listTag->payload->listTag.list, prevAllocSize); // copy previous array into new array
		pfn_free(listTag->payload->listTag.list);
		listTag->payload->listTag.list = pPayloads;
	}
	else {
		listTag->payload->listTag.list = (nbt_tag_payload_t*)pfn_malloc(allocSizeTable[listTag->payload->listTag.listType]);
	}

	switch (listTag->payload->listTag.listType) {
		case NBT_TAG_TYPE_BYTE: {
			((int8_t*)(listTag->payload->listTag.list))[listTag->payload->listTag.listSize] = payload.byteTag;
		} break;
		case NBT_TAG_TYPE_SHORT: {
			((int16_t*)listTag->payload->listTag.list)[listTag->payload->listTag.listSize] = payload.shortTag;
		} break;
		case NBT_TAG_TYPE_INT: {
			((int32_t*)listTag->payload->listTag.list)[listTag->payload->listTag.listSize] = payload.intTag;
		} break;
		case NBT_TAG_TYPE_LONG: {
			((int64_t*)listTag->payload->listTag.list)[listTag->payload->listTag.listSize] = payload.longTag;
		} break;
		case NBT_TAG_TYPE_FLOAT: {
			((float*)listTag->payload->listTag.list)[listTag->payload->listTag.listSize] = payload.floatTag;
		} break;
		case NBT_TAG_TYPE_DOUBLE: {
			((double*)listTag->payload->listTag.list)[listTag->payload->listTag.listSize] = payload.doubleTag;
		} break;
	}
	
	listTag->payload->listTag.listSize++; // increment size
}

void nbt_list_remove_tag(nbt_tag listTag, uint32_t index) {
	if (listTag->type != NBT_TAG_TYPE_LIST) {
		return;
	}

	// realloc array
	size_t allocSize = (listTag->payload->listTag.listSize - 1) * allocSizeTable[listTag->payload->listTag.listType];
	nbt_tag_payload_t* pPayloads = (nbt_tag_payload_t*)pfn_malloc(allocSize);
	for (int32_t i = 0; i < listTag->payload->compoundTag.size; i++) {
		if (i == index) {
			continue;
		}

		pPayloads[i] = listTag->payload->listTag.list[i];
	}

	pfn_free(listTag->payload->listTag.list);
	listTag->payload->listTag.list = pPayloads;
	listTag->payload->listTag.listSize--;
}

void nbt_list_get_tag(nbt_tag listTag, uint32_t index, nbt_tag_payload_t** ppPayload) {
	for (int32_t i = 0; i < listTag->payload->listTag.listSize; i++) {
		if (i == index) {
			*ppPayload = &listTag->payload->listTag.list[i];
			return;
		}
	}
}

static void write_tag(gzFile file, nbt_tag tag);
static void write_payload(gzFile file, nbt_tag_type_t type, nbt_tag_payload_t* payload);

static void write_list_contents(gzFile file, nbt_tag_type_t type, nbt_tag_payload_t* payload) {
	char* listToArray = payload->listTag.list;
	int32_t size = payload->listTag.listSize;
	for (int32_t i = 0; i < size; i++) {
		write_payload(file, type, listToArray + (i * allocSizeTable[type]));
	}
}

static void write_payload(gzFile file, nbt_tag_type_t type, nbt_tag_payload_t* payload) {
	switch (type) {
		case NBT_TAG_TYPE_BYTE: {
			gzwrite(file, (char*)&payload->byteTag, 1);
		} break;
		case NBT_TAG_TYPE_SHORT: {
			int16_t num = (int16_t)BYTE_SWAP16((uint16_t)payload->shortTag);
			gzwrite(file, (char*)&num, 2);
		} break;
		case NBT_TAG_TYPE_INT: {
			int32_t num = (int32_t)BYTE_SWAP32((uint32_t)payload->intTag);
			gzwrite(file, (char*)&num, 4);
		} break;
		case NBT_TAG_TYPE_LONG: {
			int64_t num = (int64_t)BYTE_SWAP64((uint64_t)payload->longTag);
			gzwrite(file, (char*)&num, 8);
		} break;
		case NBT_TAG_TYPE_FLOAT: {
			uint32_t num = BYTE_SWAP32(*(uint32_t*)&payload->floatTag);
			float result = *(float*)&num;
			gzwrite(file, (char*)&result, 4);
		} break;
		case NBT_TAG_TYPE_DOUBLE: {
			uint64_t num = BYTE_SWAP64(*(uint64_t*)&payload->doubleTag);
			double result = *(double*)&num;
			gzwrite(file, (char*)&result, 8);
		} break;
		case NBT_TAG_TYPE_STRING: {
			uint32_t strLength = (uint32_t)strlen(payload->stringTag.string);
			int16_t len = (int16_t)BYTE_SWAP16((uint16_t)strLength);
			gzwrite(file, (char*)&len, 2);
			gzwrite(file, (char*)&payload->stringTag.string, strLength);
		} break;
		case NBT_TAG_TYPE_LIST: {
			gzwrite(file, (char*)&payload->listTag.listType, 1);
			int32_t size = (int32_t)BYTE_SWAP32((uint32_t)payload->listTag.listSize);
			gzwrite(file, (char*)&size, 4);
			write_list_contents(file, payload->listTag.listType, &payload->listTag.list);
		} break;
		case NBT_TAG_TYPE_COMPOUND: {
			for (int32_t i = 0; i < payload->compoundTag.size; i++) {
				write_tag(file, payload->compoundTag.tags[i]);
			}
			int8_t end = 0;
			gzwrite(file, (char*)&end, 1);
		} break;
	}
}

static void write_tag(gzFile file, nbt_tag tag) {
	gzwrite(file, (char*)&tag->type, 1); // write the tag type
	uint32_t namelen = (uint32_t)strlen(tag->name);
	int16_t len = (int16_t)BYTE_SWAP16(namelen);
	gzwrite(file, (char*)&len, 2); // write the length of the name
	gzwrite(file, (char*)tag->name, namelen); // write the name
	write_payload(file, tag->type, tag->payload); // write the tag payload
}

void nbt_write_tag_to_file(nbt_tag tag, const char* filename) {
	if (tag->type != NBT_TAG_TYPE_COMPOUND) {
		return;
	}

	gzFile file = gzopen(filename, "wb");
	if (!file) {
		return;
	}

	write_tag(file, tag);

	gzclose(file);
}

