#include "Window.h"
#include <LinkedList.h>

#include <string.h>

namespace MarkTech
{
#ifdef MT_PLATFORM_WINDOWS
	const wchar_t WINDOW_CLASS_NAME[] = L"MarkTechWindow";
	HINSTANCE ghApplicationInstance = GetModuleHandle(NULL);

	U32 gWindowModeTable[3] = {
		{ 0 },
		{ WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX },
		{ WS_POPUP /* | WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX*/ }
	};

	LinkedList<Window> gWindowList = {};

	static PFN_WINDOWEVENTHANDLER GetEventHandler(HWND hWnd)
	{
		LinkedList<Window>::NodeType* pNode = gWindowList.pStart;
		while (pNode != nullptr)
		{
			if (pNode->data.hWnd == hWnd)
			{
				return pNode->data.pfnEventHandler;
			}

			pNode = pNode->pNext;
		}

		return nullptr;
	}

	LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		PFN_WINDOWEVENTHANDLER pfnHandler = GetEventHandler(hWnd);

		switch (uMsg)
		{
		case WM_CLOSE: 
		{
			if (pfnHandler)
			{
				WindowEventCloseInfo info = {};
				info.type = WINDOW_EVENT_CLOSE;
				pfnHandler(&info);
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
				if (pfnHandler)
					pfnHandler(&info);
			}
		} break;
		case WM_MOUSEMOVE: 
		{
			if (pfnHandler)
			{
				WindowEventMouseMoveInfo info = {};
				info.type = WINDOW_EVENT_MOUSEPOS;
				info.x = LOWORD(wParam);
				info.y = HIWORD(lParam);
				pfnHandler(&info);
			}
		} break;
		case WM_LBUTTONDOWN:
		{
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = 0b1;
				pfnHandler(&info);
			}
		} break;
		case WM_LBUTTONUP: 
		{
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = 0b1;
				pfnHandler(&info);
			}
		} break;
		case WM_MBUTTONDOWN:
		{ 
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = 0b10;
				pfnHandler(&info);
			}
		} break;
		case WM_MBUTTONUP: 
		{ 
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = 0b10;
				pfnHandler(&info);
			}
		} break;
		case WM_RBUTTONDOWN: 
		{
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = 0b100;
				pfnHandler(&info);
			}
		} break;
		case WM_RBUTTONUP: 
		{ 
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = 0b100;
				pfnHandler(&info);
			}
		} break;
		case WM_XBUTTONDOWN: 
		{
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_DOWN;
				info.buttons = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 0b10000 : 0b1000;
				pfnHandler(&info);
			}
		} break;
		case WM_XBUTTONUP: {
			if (pfnHandler)
			{
				WindowEventMouseButtonsInfo info = {};
				info.type = WINDOW_EVENT_MOUSEBUTTON_UP;
				info.buttons = GET_XBUTTON_WPARAM(wParam) == XBUTTON1 ? 0b10000 : 0b1000;
				pfnHandler(&info);
			}
		} break;
		}

		switch (uMsg)
		{
		case WM_DESTROY: { PostQuitMessage(0); } return 0;
		case WM_CLOSE: { DestroyWindow(hWnd); } return 0;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void ConstructWindow(const WindowInfo& info, Window& window)
	{
		if (!gWindowList.pStart)
		{
			CreateLinkedList(gWindowList, nullptr);

			WNDCLASSW winClass = {};
			winClass.lpfnWndProc = WindowProc;
			winClass.lpszClassName = WINDOW_CLASS_NAME;
			winClass.hInstance = ghApplicationInstance;

			RegisterClass(&winClass);
		}

		window.defaultMode = info.defaultMode;
		window.height = info.height;
		window.pfnEventHandler = info.pfnEventHandler;
		window.pTitle = info.pTitle;
		window.width = info.width;

		wchar_t windowTitle[256] = L"";
		size_t sizeConverted = 0;
		mbstowcs_s(&sizeConverted, windowTitle, info.pTitle, strlen(info.pTitle));

		U32 width = info.defaultMode == WINDOW_MODE_BORDERLESS_WINDOWED ? GetSystemMetrics(SM_CXSCREEN) : info.width;
		U32 height = info.defaultMode == WINDOW_MODE_BORDERLESS_WINDOWED ? GetSystemMetrics(SM_CYSCREEN) : info.height;

		window.hWnd = CreateWindowW(
			WINDOW_CLASS_NAME,
			windowTitle, 
			gWindowModeTable[info.defaultMode] | CS_OWNDC, 
			CW_USEDEFAULT, 
			CW_USEDEFAULT, 
			width,
			height,
			NULL, NULL, NULL, NULL);

		if (window.hWnd == NULL)
			return;

		ShowWindow(window.hWnd, SW_NORMAL);
		InsertLinkedListItem(gWindowList, window);
	}

	void ReleaseWindow(Window& window)
	{
		DestroyWindow(window.hWnd);
		UnregisterClassW(WINDOW_CLASS_NAME, ghApplicationInstance);
		
		LinkedList<Window>::NodeType* pNode = gWindowList.pStart;
		while (pNode != nullptr)
		{
			if (pNode->data.hWnd == window.hWnd)
			{
				RemoveLinkedListItem(gWindowList, pNode);
				break;
			}

			pNode = pNode->pNext;
		}
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

	void WindowMessageBox(const Window& window, const wchar_t* pHeader, const wchar_t* pDesc)
	{
		MessageBoxW(window.hWnd, pDesc, pHeader, MB_ICONERROR);
	}
#endif
}