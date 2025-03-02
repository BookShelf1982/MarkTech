#include "window.h"
#include "imgui_impl_win32.h"

#ifdef _WINDOWS
#define UNICODE
#include <Windows.h>
#define WNDCLASS_NAME L"mzwkwnd"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam); // Use ImGui::GetCurrentContext()

LRESULT wndproc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam
)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
		return true;

	switch (msg)
	{
	case WM_CLOSE: 
	{
		PFN_WindowClose pfn = (PFN_WindowClose)GetWindowLongPtrW(hWnd, 0);
		if (pfn)
			pfn();
		return 0;
	}
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
	{
		PFN_WindowKeyChanged pfn = (PFN_WindowKeyChanged)GetWindowLongPtrW(hWnd, 8);
		if (pfn)
			pfn((uint32_t)wParam);
		return 0;
	}
	case WM_KEYUP:
	case WM_SYSKEYUP:
	{
		PFN_WindowKeyChanged pfn = (PFN_WindowKeyChanged)GetWindowLongPtrW(hWnd, 16);
		if (pfn)
			pfn((uint32_t)wParam);
		return 0;
	}
	}

	return DefWindowProcW(hWnd, msg, wParam, lParam);
}
#endif

uint32_t g_activeWindows = 0;

#undef CreateWindow
void CreateWindow(const char* title, uint32_t width, uint32_t height, Window& window)
{
#ifdef _WINDOWS
	// if this is the first window then register the wndclass
	if (g_activeWindows == 0) {
		WNDCLASSW wc;
		wc.style = 0;
		wc.lpfnWndProc = wndproc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 8 * 4;
		wc.hInstance = GetModuleHandleW(NULL);
		wc.hIcon = NULL;
		wc.hCursor = LoadCursorW(NULL, IDC_ARROW);
		wc.hbrBackground = NULL;
		wc.lpszMenuName = NULL;
		wc.lpszClassName = WNDCLASS_NAME;

		RegisterClassW(&wc);
	}

	wchar_t wtitle[32] = L"";
	size_t chars_converted = 0;
	mbstowcs_s(&chars_converted, wtitle, 32, title, 32);
	window._handle = (uintptr_t)CreateWindowW(WNDCLASS_NAME, wtitle, WS_OVERLAPPEDWINDOW | CS_OWNDC, CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, NULL, 0);
	ShowWindow((HWND)window._handle, SW_SHOWDEFAULT);
#endif

	g_activeWindows++;
}

void ReleaseWindow(Window& window)
{
#ifdef _WINDOWS
	DestroyWindow((HWND)window._handle);
	g_activeWindows--;
	if (g_activeWindows == 0) {
		// unregister class if there are no more windows
		UnregisterClassW(WNDCLASS_NAME, NULL);
	}
#endif
}

void PollWindowEvents(Window& window)
{
#ifdef _WINDOWS
	// proccess all messages in the queue
	MSG msg;
	while (PeekMessageW(&msg, (HWND)window._handle, 0, 0, PM_REMOVE) != 0)
	{
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
#endif
}

void SetCloseCallback(Window& window, PFN_WindowClose callback)
{
#ifdef _WINDOWS
	SetWindowLongPtrW((HWND)window._handle, 0, (LONG_PTR)callback);
#endif
}

void SetKeyPressedCallback(Window& window, PFN_WindowKeyChanged callback)
{
#ifdef _WINDOWS
	SetWindowLongPtrA((HWND)window._handle, 8, (LONG_PTR)callback);
#endif
}

void SetKeyReleasedCallback(Window& window, PFN_WindowKeyChanged callback)
{
#ifdef _WINDOWS
	SetWindowLongPtrA((HWND)window._handle, 16, (LONG_PTR)callback);
#endif
}