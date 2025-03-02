#include "dev_gui.h"

namespace MarkTech
{
	void InitImGui(const Window& window)
	{
		ImGui::CreateContext();
		ImGui_ImplWin32_Init((void*)window._handle);
		ImGui_ImplOpenGL3_Init();
	}

	void NewFrameImGui()
	{
		ImGui_ImplWin32_NewFrame();
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();
	}

	void RenderFrameImGui()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ShutdownImGui()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}