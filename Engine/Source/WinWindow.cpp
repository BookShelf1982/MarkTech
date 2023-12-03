#include "WinWindow.h"

namespace MarkTech
{
    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    CWinWindow::CWinWindow()
    {
        m_Hwnd = NULL;
        m_WC = { };
    }

    CWinWindow::~CWinWindow()
    {
        DestroyWindow(m_Hwnd);
        UnregisterClassW(m_WC.lpszClassName, m_WC.hInstance);
    }

    void CWinWindow::CreateWinWindow(LPCWSTR ClassName, LPCWSTR WindowName, int iPosX, int iPosY, int iWidth, int iLength, HINSTANCE hInstance, int nCmdShow)
    {
        m_WC.lpfnWndProc = WindowProc;
        m_WC.hInstance = hInstance;
        m_WC.lpszClassName = ClassName;

        RegisterClass(&m_WC);

        nWidth = iWidth;
        nHeight = iLength;
        m_Hwnd = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, iPosX, iPosY, iWidth, iLength, NULL, NULL, hInstance, NULL);

        ShowWindow(m_Hwnd, nCmdShow);
        UpdateWindow(m_Hwnd);
    }

    void CWinWindow::CreateErrorBox(const LPCWSTR ErrorString)
    {
        MessageBox(m_Hwnd, ErrorString, L"MARKTECH FATAL ERROR!", MB_ICONWARNING);
    }

    LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
        switch (uMsg)
        {
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            return 0;
        }break;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }


    void CWinWindow::MessageLoop(MSG msg)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);

        switch (msg.message)
        {
        case WM_SIZE:
        {
            nWidth = LOWORD(msg.lParam);
            nHeight = HIWORD(msg.lParam);
        }break;
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            uint32_t keycode = (uint32_t)msg.wParam;
            bool bWasDown = (msg.lParam & BIT(30)) != 0;
            bool bIsDown = (msg.lParam & BIT(31)) == 0;
            CInput::GetInput()->PollInput(keycode, bIsDown, bWasDown);
        }break;
        case WM_MOUSEMOVE:
        {
            CInput::GetInput()->PollMousePos(GET_X_LPARAM(msg.lParam), GET_Y_LPARAM(msg.lParam));
        }break;
        case WM_LBUTTONUP:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONUP:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONUP:
        case WM_MBUTTONDOWN:
        case WM_XBUTTONUP:
        case WM_XBUTTONDOWN:
        {
            CInput::GetInput()->PollMouseInput(msg.wParam);
        }break;
        }
    }
}