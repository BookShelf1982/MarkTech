#include "djb2.h"

uint32_t djb2(const char* str)
{
	uint32_t hash = 5381;
	int32_t c = 0;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}