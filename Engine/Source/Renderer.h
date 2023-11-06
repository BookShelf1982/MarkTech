#pragma once
#include "DllExport.h"
#include "Core.h"
#include "MModel.h"
#include <Windows.h>

#define MAX_VERTICES_PER_DRAWCALL 1024
#define MAX_INDICES_PER_DRAWCALL 3072

namespace MarkTech
{
	namespace Renderer
	{
		class MARKTECH_API CRenderer
		{
		public:
			virtual bool InitRenderer(HWND hwnd) { return false; };

			virtual void CreateShaders() {};

			virtual void RenderFrame(HWND hwnd) {};
		};
	}
}