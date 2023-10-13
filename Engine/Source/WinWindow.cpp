#include "WinWindow.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CWinWindow::CWinWindow()
{
    hwnd = NULL;
}

void CWinWindow::CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow)
{
    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = ClassName;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, iPosX, iPosY, iWidth, iLength, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
}

void CWinWindow::CreateWinMessageBox()
{
    MessageBox(hwnd, L"Unable to find GameInfo.ini", L"MARKTECH FATAL ERROR!", MB_ICONWARNING);
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}


void CWinWindow::MessageLoop(MSG msg)
{
    TranslateMessage(&msg);
    DispatchMessage(&msg);
}