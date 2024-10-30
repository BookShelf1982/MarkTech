#include "Window.h"
namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	const wchar_t WINDOW_CLASS_NAME[] = L"MarkTechWindow";
	HINSTANCE hInst = GetModuleHandle(NULL);
	PFN_WINDOWEVENTHANDLER pfnEventHandler = nullptr;

	LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_CLOSE: 
		{
			if (pfnEventHandler)
			{
				WindowEventCloseInfo info = {};
				info.type = WINDOW_EVENT_CLOSE;
				pfnEventHandler(&info);
			}
		} break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP: {
			bool keyDown, keyWasDown;
			keyDown = ((lParam & (1 << 31)) == 0);
			keyWasDown = ((lParam & (1 << 30)) != 0);
			if (keyDown != keyWasDown)
			{
				WindowEventKeyChangedInfo info = {};
				info.type = WINDOW_EVENT_KEYCHANGED;
				info.keydown = keyDown;
				info.keycode = wParam;
				if (pfnEventHandler)
					pfnEventHandler(&info);
			}
		} break;
		case WM_MOUSEMOVE: 
		{
			if (pfnEventHandler)
			{
				WindowEventMouseMoveInfo info = {};
				info.type = WINDOW_EVENT_MOUSEPOS;
				info.x = LOWORD(wParam);
				info.y = HIWORD(lParam);
				pfnEventHandler(&info);
			}
		} break;
		case WM_LBUTTONDOWN:
		{
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = 0b1;
				pfnEventHandler(&info);
			}
		} break;
		case WM_LBUTTONUP: 
		{
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = 0b1;
				pfnEventHandler(&info);
			}
		} break;
		case WM_MBUTTONDOWN:
		{ 
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = 0b10;
				pfnEventHandler(&info);
			}
		} break;
		case WM_MBUTTONUP: 
		{ 
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = 0b10;
				pfnEventHandler(&info);
			}
		} break;
		case WM_RBUTTONDOWN: 
		{
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = 0b100;
				pfnEventHandler(&info);
			}
		} break;
		case WM_RBUTTONUP: 
		{ 
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = 0b100;
				pfnEventHandler(&info);
			}
		} break;
		case WM_XBUTTONDOWN: 
		{
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 0b10000 : 0b1000;
				pfnEventHandler(&info);
			}
		} break;
		case WM_XBUTTONUP: {
			if (pfnEventHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 0b10000 : 0b1000;
				pfnEventHandler(&info);
			}
		} break;
		}

		switch (uMsg)
		{
		case WM_DESTROY: { PostQuitMessage(0); } return 0;
		case WM_CLOSE: { DestroyWindow(hwnd); } return 0;
		}
		return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}

	Window MakeWindow(const wchar_t* pTitle, U16 width, U16 height)
	{
		Window window = {};

		WNDCLASSW winClass = {};
		winClass.lpfnWndProc = WindowProc;
		winClass.lpszClassName = WINDOW_CLASS_NAME;
		winClass.hInstance = hInst;

		RegisterClass(&winClass);

		window.hWnd = CreateWindowW(WINDOW_CLASS_NAME, pTitle, WS_OVERLAPPEDWINDOW | CS_OWNDC, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInst, NULL);
		if (window.hWnd == NULL)
			return window;

		ShowWindow(window.hWnd, SW_NORMAL);

		return window;
	}

	void KillWindow(const Window* pWindow)
	{
		DestroyWindow(pWindow->hWnd);
		UnregisterClassW(WINDOW_CLASS_NAME, hInst);
	}

	void PollWindowMessages()
	{
		MSG msg = { };
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void SetWindowEventHandler(PFN_WINDOWEVENTHANDLER eventHandler)
	{
		pfnEventHandler = eventHandler;
	}

	void WindowMessage(const Window* pWindow, const wchar_t* pHeader, const wchar_t* pDesc)
	{
		if (pWindow)
		{
			MessageBox(pWindow->hWnd, pDesc, pHeader, MB_OK);
		}
		else
		{
			MessageBox(NULL, pDesc, pHeader, MB_OK | MB_ICONEXCLAMATION);
		}
	}

#endif
}