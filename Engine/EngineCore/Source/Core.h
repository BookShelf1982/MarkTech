#pragma once
#include <stdint.h>
#include "StringType.h"
#include "MemoryPool.h"
#include <assert.h>
#include <crtdbg.h>

typedef unsigned char byte;

#ifdef DEBUG
#define MYDEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__)
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define MYDEBUG_NEW
#endif // _DEBUG
// MyApp.cpp
// Compile options needed: /Zi /D_DEBUG /MLd
// or use a
// Default Workspace for a Console Application to
// build a Debug version
#include "crtdbg.h"

#ifdef DEBUG
#define new MYDEBUG_NEW
#endif