#pragma once
#include "GraphicsInterface.h"
#include "Window.h"

namespace MarkTech
{
	struct Renderer2D
	{
		GraphicsContext context;
	};

	Renderer2D InitRenderer2D(const Window* pWindow);
	void ShutdownRenderer2D(Renderer2D* pRenderer);
}