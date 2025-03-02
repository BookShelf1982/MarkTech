#pragma once

#include <stdlib.h>

#define KiB(x) x * 1024
#define MiB(x) x * 1048576

inline void* AlignedAlloc(size_t size, size_t alignment) { return _aligned_malloc(size, alignment); }
inline void* AlignedRealloc(void* ptr, size_t size, size_t alignment) { return _aligned_realloc(ptr, size, alignment); }
inline void AlignedFree(void* ptr) { _aligned_free(ptr); }