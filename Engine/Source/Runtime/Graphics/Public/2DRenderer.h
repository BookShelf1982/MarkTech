#pragma once
#include "GraphicsInterface.h"

namespace MarkTech
{
	struct Renderer2D
	{
		GraphicsContext context;
	};

	Renderer2D InitRenderer2D();
	void ShutdownRenderer2D(Renderer2D* pRenderer);
}