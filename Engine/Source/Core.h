#pragma once
#ifdef DEBUG
#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
//allocations to be of _CLIENT_BLOCK type
#else
#define MYDEBUG_NEW
#endif // _DEBUG
/* MyApp.cpp
/* Compile options needed: /Zi /D_DEBUG /MLd
/* or use a
/* Default Workspace for a Console Application to
/* build a Debug version*/
#include "crtdbg.h"

#ifdef DEBUG
#define new MYDEBUG_NEW
#endif

#include <stdint.h>
#include <string.h>
#include "Array.h"
#include "MemoryBlob.h"
#include "MString.h"
#pragma warning(disable : 4251)

#define BIT(x) (1 << x)

MAKE_CTARRAY_COMPATIBLE(int);
MAKE_CTARRAY_COMPATIBLE(float);



