#include "window.h"

#ifdef _WIN32
#include <Windows.h>
#define WNDCLASS_NAME L"mzwkwnd"
LRESULT wndproc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	switch (msg) {
	case WM_CLOSE: {
		PFN_windowClose pfn = (PFN_windowClose)GetWindowLongPtrW(hWnd, 0);
		if (pfn)
			pfn();
		return 0;
	}
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}
#endif

uint32_t g_activeWindows = 0;

#undef CreateWindow
void CreateWindow(const char* title, uint32_t width, uint32_t height, Window* window)
{
	// if this is the first window then register the wndclass
	if (g_activeWindows == 0) {
		WNDCLASSW wc;
		wc.style = 0;
		wc.lpfnWndProc = wndproc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = sizeof(PFN_windowClose);
		wc.hInstance = GetModuleHandleW(NULL);
		wc.hIcon = NULL;
		wc.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(230));
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WNDCLASS_NAME;

		RegisterClassW(&wc);
	}

#ifdef _WIN32
	wchar_t wtitle[32] = L"";
	size_t chars_converted = 0;
	mbstowcs_s(&chars_converted, wtitle, 32, title, 32);
	window->_handle = (uintptr_t)CreateWindowW(WNDCLASS_NAME, wtitle, WS_OVERLAPPEDWINDOW | CS_OWNDC, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, 0);
	ShowWindow((HWND)window->_handle, SW_SHOWDEFAULT);
#endif

	g_activeWindows++;
}

void ReleaseWindow(Window* window)
{
#ifdef _WIN32
	DestroyWindow((HWND)window->_handle);
	g_activeWindows--;
	if (g_activeWindows == 0) {
		UnregisterClassW(WNDCLASS_NAME, NULL); // unregister class if there are no more windows
	}
#endif
}

void PollWindowEvents(Window* window)
{
#ifdef _WIN32
	MSG msg;
	while (PeekMessageW(&msg, (HWND)window->_handle, 0, 0, PM_REMOVE) != 0) // proccess all messages in the queue
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
#endif
}

void SetCloseCallback(Window* window, PFN_windowClose callback)
{
#ifdef _WIN32
	SetWindowLongPtrW((HWND)window->_handle, 0, (LONG_PTR)callback);
#endif
}