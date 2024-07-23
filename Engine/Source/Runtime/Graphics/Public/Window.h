#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum class WindowEvent
	{
		WINDOW_CLOSE
	};

	typedef void (*PFN_WINDOWEVENTHANDLER)(WindowEvent);

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
}