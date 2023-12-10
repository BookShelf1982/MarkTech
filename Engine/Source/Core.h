#pragma once
#include <stdint.h>
#include <string.h>
#include "Array.h"
#include "MemoryBlob.h"
#include "MString.h"
#pragma warning(disable : 4251)

#define BIT(x) (1 << x)

MAKE_CTARRAY_COMPATIBLE(int);
MAKE_CTARRAY_COMPATIBLE(float);

