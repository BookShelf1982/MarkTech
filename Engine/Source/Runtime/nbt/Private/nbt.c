#include "nbt.h"
#include <zlib.h>

#include <stdlib.h>
#include <string.h>

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
	nbt_tag tag = (nbt_tag)malloc(sizeof(nbt_tag_t));
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

