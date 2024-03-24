#pragma once
#include "Core.h"
#include <GLFW\glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW\glfw3native.h>

typedef GLFWwindow* (*PFN_CreateWindow)(int, int, const char*);
typedef void (*PFN_DestroyWindow)(GLFWwindow*);
typedef void (*PFN_SetWindowCloseCallback)(GLFWwindow*, GLFWwindowclosefun);
typedef void (*PFN_SetWindowUserPointer)(GLFWwindow*, void*);
typedef void* (*PFN_GetWindowUserPointer)(GLFWwindow*);
typedef HWND (*PFN_GetWindowHWND)(GLFWwindow*);
typedef void (*PFN_PollWindow)();

struct window_funcs_t
{
	PFN_CreateWindow pfnCreateWindow;
	PFN_DestroyWindow pfnDestroyWindow;
	PFN_PollWindow pfnPollWindow;
	PFN_SetWindowCloseCallback pfnSetWindowCloseCallback;
	PFN_SetWindowUserPointer pfnSetWindowUserPointer;
	PFN_GetWindowUserPointer pfnGetWindowUserPointer;
	PFN_GetWindowHWND pfnGetWindowHWND;
};

extern "C"
{
	DLLEXPORT void InitModule();
	DLLEXPORT void UpdateModule();
	DLLEXPORT void ShutdownModule();

	DLLEXPORT GLFWwindow* CreateGLFWWindow(int nWidth, int nHeight, const char* pTitle);
	DLLEXPORT void PollWindow();
	DLLEXPORT void DestroyGLFWWindow(GLFWwindow* pWindow);
	DLLEXPORT void SetWindowCloseCallback(GLFWwindow* pWindow, GLFWwindowclosefun callback);
	DLLEXPORT void SetWindowCloseCallback(GLFWwindow* pWindow, GLFWwindowclosefun callback);
	DLLEXPORT void SetWindowUserPointer(GLFWwindow* pWindow, void* ptr);
	DLLEXPORT void* GetWindowUserPointer(GLFWwindow* pWindow);
	DLLEXPORT HWND GetWindowHWND(GLFWwindow* pWindow);
}