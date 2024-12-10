#include "Window.h"

#include <string.h>

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
#define MT_CLASSNAME L"mtGameWindow"

	HINSTANCE g_hInstance = NULL;
	U32 g_windowStyleTable[3] = {
		{ WS_POPUP },
		{ WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZE },
		{ WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX },
	};

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE:
		{
			PFN_WindowEventHandler pfnHandler = (PFN_WindowEventHandler)GetWindowLongPtr(hwnd, 0);
			WindowEventType type = WINDOW_EVENT_TYPE_CLOSE;
			pfnHandler(&type);
		} return 0;
		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		{
			PFN_WindowEventHandler pfnHandler = (PFN_WindowEventHandler)GetWindowLongPtr(hwnd, 0);
			WindowKeyChangedEvent keyDown = { WINDOW_EVENT_TYPE_KEYDOWN, wParam };
			pfnHandler(&keyDown);
		} return 0;
		case WM_SYSKEYUP:
		case WM_KEYUP:
		{
			PFN_WindowEventHandler pfnHandler = (PFN_WindowEventHandler)GetWindowLongPtr(hwnd, 0);
			WindowKeyChangedEvent keyUp = { WINDOW_EVENT_TYPE_KEYUP, wParam };
			pfnHandler(&keyUp);
		} return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	Window MakeWindow(const WindowProps& props)
	{
		if (!g_hInstance)
			g_hInstance = GetModuleHandleW(NULL);

		Window window;

		WNDCLASSW wc;
		wc.style = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = MT_CLASSNAME;
		wc.lpfnWndProc = WindowProc;
		wc.hInstance = g_hInstance;
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(g_hInstance, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.cbWndExtra = sizeof(PFN_WindowEventHandler);
		wc.cbClsExtra = 0;

		RegisterClass(&wc);

		wchar_t convertedTitle[256] = L"";
		size_t size = 0;
		mbstowcs_s(&size, convertedTitle, props.pTitle, strlen(props.pTitle));
		window.hWnd = CreateWindow(
			MT_CLASSNAME,
			convertedTitle,
			g_windowStyleTable[props.style],
			CW_USEDEFAULT,
			CW_USEDEFAULT,
			props.width,
			props.height,
			NULL,
			NULL,
			g_hInstance,
			NULL
		);

		ShowWindow(window.hWnd, SW_SHOWDEFAULT);

		SetWindowLongPtr(window.hWnd, 0, (LONG_PTR)props.eventHandler);

		return window;
	}

	void PollWindowEvents(Window& window)
	{
		MSG msg;
		while (PeekMessage(&msg, window.hWnd, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void KillWindow(Window& window)
	{
		DestroyWindow(window.hWnd);
		UnregisterClassW(MT_CLASSNAME, g_hInstance);
	}

#endif
}