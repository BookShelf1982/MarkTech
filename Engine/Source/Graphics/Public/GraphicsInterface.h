#pragma once
#include "Core.h"
#include "GraphicsContext.h"
#include <Windows.h>

typedef IGraphicsContext* (*PFN_CreateGraphicsContext)(EGraphicsAPI, HWND);
typedef void (*PFN_DestroyGraphicContext)(IGraphicsContext*);

struct graphics_funcs_t
{
	PFN_CreateGraphicsContext pfnCreateGraphicsContext;
	PFN_DestroyGraphicContext pfnDestroyGraphicsContext;
};

extern "C"
{
	DLLEXPORT void InitModule();

	DLLEXPORT void UpdateModule();

	DLLEXPORT void ShutdownModule();

	DLLEXPORT IGraphicsContext* CreateGraphicsContext(EGraphicsAPI api, HWND hwnd);
	DLLEXPORT void DestroyGraphicsContext(IGraphicsContext* pGraphicsContext);
}