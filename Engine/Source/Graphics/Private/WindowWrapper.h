#pragma once
#include <GLFW\glfw3.h>
#include "ModuleInterface.h"

class CWindowWrapper
{
public:
	static void CreateWindow(const char* pTitle, uint32_t nWidth, uint32_t nHeight);
	static void PollWindow();
	static void CloseWindow();
private:
	static GLFWwindow* m_pWindow;
};