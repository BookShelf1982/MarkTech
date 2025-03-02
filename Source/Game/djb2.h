#pragma once
#include <stdint.h>

uint32_t djb2(const char* str);
inline uint32_t Hash(const char* str) { return djb2(str); }