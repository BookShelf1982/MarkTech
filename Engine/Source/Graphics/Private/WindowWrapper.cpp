#include "WindowWrapper.h"

GLFWwindow* CWindowWrapper::m_pWindow = nullptr;

void CWindowWrapper::CreateWindow(const char* pTitle, uint32_t nWidth, uint32_t nHeight)
{
	glfwInit();
	m_pWindow = glfwCreateWindow(nWidth, nHeight, pTitle, nullptr, nullptr);
}

void CWindowWrapper::PollWindow()
{
	glfwPollEvents();
}

void CWindowWrapper::CloseWindow()
{
	glfwDestroyWindow(m_pWindow);
	glfwTerminate();
}

void CreateWindow(const char* pTitle, uint32_t nWidth, uint32_t nHeight)
{
	CWindowWrapper::CreateWindow(pTitle, nWidth, nHeight);
}

void PollWindowEvents()
{
	CWindowWrapper::PollWindow();
}

void CloseWindow()
{
	CWindowWrapper::CloseWindow();
}