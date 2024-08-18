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
		case WM_CLOSE: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_CLOSE, 0, 0); } break;
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP: {
			bool keyDown, keyWasDown;
			keyDown = ((lParam & (1 << 31)) == 0);
			keyWasDown = ((lParam & (1 << 30)) != 0);
			if (keyDown != keyWasDown)
			{
				if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_KEYCHANGED, wParam, keyDown);
			}
		} break;
		case WM_MOUSEMOVE: {
			if (pfnEventHandler) pfnEventHandler(WindowEvent::WNIDOW_MOUSEPOS, LOWORD(wParam), HIWORD(lParam));
		} break;
		case WM_LBUTTONDOWN: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b1, 1); } break;
		case WM_LBUTTONUP: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b1, 0); } break;
		case WM_MBUTTONDOWN: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b10, 1); } break;
		case WM_MBUTTONUP: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b10, 0); } break;
		case WM_RBUTTONDOWN: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b100, 1); } break;
		case WM_RBUTTONUP: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b100, 0); } break;
		case WM_XBUTTONDOWN: { 
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b1000, 1);
			}
			else
			{
				if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b10000, 1);
			}
		} break;
		case WM_XBUTTONUP: {
			if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)
			{
				if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b1000, 0);
			}
			else
			{
				if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_MOUSEBUTTON, 0b10000, 0);
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

		window.hWnd = CreateWindow(WINDOW_CLASS_NAME, L"MarkTech", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInst, NULL);
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

#endif
}