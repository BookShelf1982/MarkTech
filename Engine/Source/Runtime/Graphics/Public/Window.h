#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum class WindowEvent
	{
		WINDOW_CLOSE,
		WINDOW_KEYCHANGED,
		WNIDOW_MOUSEPOS,
		WINDOW_MOUSEBUTTON
	};

	typedef void (*PFN_WINDOWEVENTHANDLER)(WindowEvent, U64, U64);

	struct Window
	{
#ifdef MT_PLATFORM_WINDOWS
		HWND hWnd;
#endif
	};

	Window MakeWindow(const wchar_t* pTitle, U16 width, U16 height);
	void KillWindow(const Window* pWindow);
	void PollWindowMessages();
	void SetWindowEventHandler(PFN_WINDOWEVENTHANDLER eventHandler);
	void WindowMessage(const Window* pWindow, const wchar_t* pHeader, const wchar_t* pDesc);
}