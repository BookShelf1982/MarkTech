#pragma once
#include "DllExport.h"
#include "Core.h"
#include "WinWindow.h"
#include "MModel.h"

#define MAX_VERTICES_PER_DRAWCALL 1024
#define MAX_INDICES_PER_DRAWCALL 3072

namespace MarkTech
{
	struct MARKTECH_API MCameraData
	{
		MVector3 camPos;
		MVector3 camTarget;
		MVector3 camUp;
		float flFov;
		float flNearZ;
		float flFarZ;
	};

	class MARKTECH_API CRenderer
	{
	public:
		virtual bool InitRenderer(const CWinWindow& window) = 0;

		virtual void CreateShaders() = 0;

		virtual void RenderFrame(const CWinWindow& window) = 0;
	};
}