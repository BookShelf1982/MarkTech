#pragma once
#include <stdint.h>

uint32_t hashString(const char* str);

constexpr uint32_t operator""_sid(const char* str, size_t size)
{
	uint32_t hash = 5381;
	int32_t c = 0;

	while (c = *str++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}