#pragma once
#include "window.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_opengl3.h"
#include "imgui.h"

namespace MarkTech
{
	void InitImGui(const Window& window);
	void NewFrameImGui();
	void RenderFrameImGui();
	void ShutdownImGui();
}