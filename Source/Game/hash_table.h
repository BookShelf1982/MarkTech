#ifndef HASH_TABLE_H
#define HAS_TABLE_H

#define DECLARE_HASHTABLE(k, v) \
typedef struct { \
	size_t tableSize; \
	struct { \
		k key; \
		v val; \
	} *table; \
} k ## _ ## v ## _ht\

#endif