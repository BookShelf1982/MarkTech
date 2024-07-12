#pragma once
#include <PrimitiveTypes.h>
#include "Window.h"

namespace MarkTech
{
	class GraphicsContext
	{
	public:
		virtual ~GraphicsContext() = default;

		static U64 GetRquiredMemSize();
		static GraphicsContext* CreateContext(Window* window);
		virtual void Destroy() = 0;
	};
}