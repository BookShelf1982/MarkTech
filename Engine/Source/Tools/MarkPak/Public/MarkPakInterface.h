#pragma once
#include <DLLUTLS.h>
#include <PrimitiveTypes.h>

typedef void (*PFN_CompileAndPackageAssets)(const char* pInputFilepath, const char* pOutputPath, const char* pPackageName);
typedef void* (*PFN_CreatePoolAllocator)(MarkTech::U64 blockSize, MarkTech::U64 blockCount);
typedef void* (*PFN_AllocFromPool)(void* pAllocator);
typedef void (*PFN_FreeToPool)(void* pAllocator, void* ptr);
typedef void (*PFN_FreePoolAllocator)(void* pAllocator);

struct MarkPakPFNList
{
	PFN_CompileAndPackageAssets pfnCompileAndPackageAssets;
	PFN_CreatePoolAllocator pfnCreatePoolAllocator;
	PFN_AllocFromPool pfnAllocFromPool;
	PFN_FreeToPool pfnFreeToPool;
	PFN_FreePoolAllocator pfnFreePoolAllocator;
};

extern MarkPakPFNList gpfnList;

typedef void (*PFN_GetMarkPakFunctions)(MarkPakPFNList* pPfnList);

MT_DLLEXPORT void GetMarkPakFunctions(MarkPakPFNList* pPfnList);