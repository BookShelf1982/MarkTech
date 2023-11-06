#include "WinWindow.h"


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

CWinWindow::CWinWindow()
{
    hwnd = NULL;
    wc = { };
}

void CWinWindow::CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow)
{
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = ClassName;

    RegisterClass(&wc);

    hwnd = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, iPosX, iPosY, iWidth, iLength, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);
}

void CWinWindow::CreateErrorBox(LPCWSTR ErrorString)
{
    MessageBox(hwnd, ErrorString, L"MARKTECH FATAL ERROR!", MB_ICONWARNING);
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