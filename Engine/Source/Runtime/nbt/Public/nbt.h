#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
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
} nbt_tag_type_t;

typedef union nbt_tag_payload_t {
	int8_t byteTag;
	int16_t shortTag;
	int32_t intTag;
	int64_t longTag;
	float floatTag;
	double doubleTag;
	struct {
		int8_t* byteArray;
		int32_t size;
	} byteArrayTag;
	struct {
		int8_t* string;
		int16_t size;
	} stringTag;
	struct {
		union nbt_tag_payload_t* list;
		nbt_tag_type_t listType;
		int32_t listSize;
	} listTag;
	struct {
		struct nbt_tag_t** tags;
		int32_t size;
	} compoundTag;
	struct {
		int32_t* intArray;
		int32_t size;
	} intArrayTag;
	struct {
		int64_t* longArray;
		int32_t size;
	} longArrayTag;
} nbt_tag_payload_t;

typedef struct nbt_tag_t {
	nbt_tag_type_t type;
	char* name;
	nbt_tag_payload_t* payload;
} nbt_tag_t;

typedef nbt_tag_t* nbt_tag;

typedef void* (*nbt_malloc_func)(size_t);
typedef void (*nbt_free_func)(void*);

typedef struct {
	nbt_malloc_func malloc;
	nbt_free_func free;
} nbt_init_info_t;

void nbt_init(const nbt_init_info_t* info);
void nbt_shutdown();

typedef struct {
	nbt_tag_type_t type;
	char* name;
	nbt_tag_payload_t payload;
} nbt_create_tag_info_t;

void nbt_create_tag(const nbt_create_tag_info_t* info, nbt_tag* pTag);
void nbt_destroy_tag(nbt_tag tag);

void nbt_compound_add_tag(nbt_tag compoundTag, nbt_tag tag);
void nbt_compound_remove_tag(nbt_tag compoundTag, char* name);
void nbt_compound_get_tag(nbt_tag compoundTag, char* name, nbt_tag* pTag);

void nbt_list_append_tag(nbt_tag listTag, nbt_tag_payload_t payload);
void nbt_list_remove_tag(nbt_tag listTag, uint32_t index);
void nbt_list_get_tag(nbt_tag listTag, uint32_t index, nbt_tag_payload_t** ppPayload);

void nbt_read_tag_from_file(nbt_tag* pTag, const char* filename);
void nbt_write_tag_to_file(nbt_tag tag, const char* filename);

#ifdef __cplusplus
}
#endif