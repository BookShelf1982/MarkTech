#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
	extern HINSTANCE ghApplicationInstance;

	enum WindowMode
	{
		WINDOW_MODE_FULLSCREEN,
		WINDOW_MODE_WINDOWED,
		WINDOW_MODE_BORDERLESS_WINDOWED
	};

	enum WindowEventType
	{
		WINDOW_EVENT_CLOSE,
		WINDOW_EVENT_KEYCHANGED,
		WINDOW_EVENT_MOUSEPOS,
		WINDOW_EVENT_MOUSEBUTTON_DOWN,
		WINDOW_EVENT_MOUSEBUTTON_UP
	};

	struct WindowEventCloseInfo
	{
		WindowEventType type;
		U32 flags; // USED FOR FUTURE USE... IDK?
	};

	struct WindowEventKeyChangedInfo
	{
		WindowEventType type;
		U64 keycode;
		bool keydown;
	};

	struct WindowEventMouseMoveInfo
	{
		WindowEventType type;
		U32 x, y;
	};

	struct WindowEventMouseButtonsInfo
	{
		WindowEventType type;
		U8 buttons;
	};

	typedef void (*PFN_WINDOWEVENTHANDLER)(void* pEvent);

	struct WindowInfo
	{
		const char* pTitle;
		U32 width;
		U32 height;
		WindowMode defaultMode;
		PFN_WINDOWEVENTHANDLER pfnEventHandler;
	};

	struct Window
	{
#ifdef MT_PLATFORM_WINDOWS
		HWND hWnd;
#endif
		PFN_WINDOWEVENTHANDLER pfnEventHandler;
		const char* pTitle;
		U32 width;
		U32 height;
		WindowMode defaultMode;
	};

	void ConstructWindow(const WindowInfo& info, Window& window);
	void ReleaseWindow(Window& pWindow);
	void PollWindowMessages();
	void WindowMessageBox(const Window& pWindow, const wchar_t* pHeader, const wchar_t* pDesc);
}