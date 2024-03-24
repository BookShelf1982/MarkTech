#include "WindowInterface.h"

bool IsSameWindow(GLFWwindow* pWindow, GLFWwindow* pOtherWindow)
{
	return pWindow == pOtherWindow;
}

void InitModule()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

void UpdateModule()
{

}

void ShutdownModule()
{
	glfwTerminate();
}

GLFWwindow* CreateGLFWWindow(int nWidth, int nHeight, const char* pTitle)
{
	return glfwCreateWindow(nWidth, nHeight, pTitle, nullptr, nullptr);
}

void PollWindow()
{
	glfwPollEvents();
}

void DestroyGLFWWindow(GLFWwindow* pWindow)
{
	glfwDestroyWindow(pWindow);
}

void SetWindowCloseCallback(GLFWwindow* pWindow, GLFWwindowclosefun callback)
{
	glfwSetWindowCloseCallback(pWindow, callback);
}

void SetWindowUserPointer(GLFWwindow* pWindow, void* ptr)
{
	glfwSetWindowUserPointer(pWindow, ptr);
}

void* GetWindowUserPointer(GLFWwindow* pWindow)
{
	return glfwGetWindowUserPointer(pWindow);
}

HWND GetWindowHWND(GLFWwindow* pWindow)
{
	return glfwGetWin32Window(pWindow);
}
