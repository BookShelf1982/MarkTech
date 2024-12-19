#pragma once
#include "PrimitiveTypes\intalias.h"
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	enum WindowMessageType
	{
		WINDOW_MESSAGE_TYPE_INFO,
		WINDOW_MESSAGE_TYPE_WARNING,
		WINDOW_MESSAGE_TYPE_ERROR
	};

	enum WindowStyle
	{
		WINDOW_STYLE_BORDERLESS,
		WINDOW_STYLE_MAXIMIZED,
		WINDOW_STYLE_WINDOWED
	};

	enum WindowEventType
	{
		WINDOW_EVENT_TYPE_CLOSE,
		WINDOW_EVENT_TYPE_KEYDOWN,
		WINDOW_EVENT_TYPE_KEYUP
	};

	struct WindowKeyChangedEvent
	{
		WindowEventType type;
		U64 keycode;
	};

	typedef void* WindowEventHandle;
	typedef void (*PFN_WindowEventHandler)(WindowEventHandle windowEvent);

	struct WindowProps
	{
		const char* pTitle;
		U32 width, height;
		WindowStyle style;
		PFN_WindowEventHandler eventHandler;
	};

	struct Window
	{
#ifdef MT_PLATFORM_WINDOWS
		HWND hWnd;
#endif
	};

	Window MakeWindow(const WindowProps& props);
	void PollWindowEvents(Window& window);
	void WindowMessageBox(Window& window, const char* pHeader, const char* pMessage, WindowMessageType type);
	void KillWindow(Window& window);
}
