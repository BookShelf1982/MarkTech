#pragma once
#include <PrimitiveTypes.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif

namespace MarkTech
{
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