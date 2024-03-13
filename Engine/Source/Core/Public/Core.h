#pragma once
#include <stdlib.h>
#include <stdint.h>

#define MASSERT(x)\
{\
	if((x) == 0) {__debugbreak(); abort();}\
}