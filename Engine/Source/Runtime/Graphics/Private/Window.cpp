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
		case WM_CLOSE: { if (pfnEventHandler) pfnEventHandler(WindowEvent::WINDOW_CLOSE); } break;
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