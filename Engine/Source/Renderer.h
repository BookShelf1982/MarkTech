#pragma once
#include "DllExport.h"
#include "Core.h"
#include "MModel.h"
#include <Windows.h>

#define MAX_VERTICES_PER_DRAWCALL 1024
#define MAX_INDICES_PER_DRAWCALL 3072

namespace MarkTech
{
	class MARKTECH_API CRenderer
	{
	public:
		virtual bool InitRenderer(HWND hwnd) = 0;

		virtual void CreateShaders() = 0;;

		virtual void RenderFrame(HWND hwnd) = 0;
	};
}