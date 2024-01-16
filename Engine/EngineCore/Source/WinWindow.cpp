#include "WinWindow.h"
#include "imgui_impl_win32.h"

CWinWindow::CWinWindow()
    :m_hInstance(NULL), m_WndClass(), m_hWnd(NULL)
{
}

CWinWindow::~CWinWindow()
{
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void CWinWindow::MakeWindow(String title, int x, int y, int width, int height, EWindowType type, CInput* inputRef)
{
    m_pInputRef = inputRef;
	m_WndClass.lpszClassName = L"MarkTechWindow";
	m_WndClass.hInstance = m_hInstance;
    m_WndClass.lpfnWndProc = WindowProc;

    RegisterClass(&m_WndClass);

    m_pszTitle = title;

    wchar_t buff[64];
    size_t size = 64;
    CONVERT_STRING_TO_WIDE_STRING(title, buff, size);

    switch (type)
    {
    case EFullscreen:
        m_hWnd = CreateWindowExW(0, m_WndClass.lpszClassName, buff, WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, x, y, width, height, NULL, NULL, NULL, NULL);
        break;
    case EBorderless:
        m_hWnd =CreateWindowExW(0, m_WndClass.lpszClassName, buff, WS_POPUP | WS_OVERLAPPED | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX, x, y, width, height, NULL, NULL, NULL, NULL);
        break;
    case EWindowed:
        m_hWnd = CreateWindowExW(0, m_WndClass.lpszClassName, buff, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, NULL, NULL);
        break;
    default:
        m_hWnd = CreateWindowExW(0, m_WndClass.lpszClassName, buff, WS_OVERLAPPEDWINDOW, x, y, width, height, NULL, NULL, NULL, NULL);
        break;
    }

    RECT rect;
    GetClientRect(m_hWnd, &rect);
    m_nWidth = rect.right - rect.left;
    m_nHeight = rect.bottom - rect.top;

    ShowWindow(m_hWnd, SW_SHOWNORMAL);
    UpdateWindow(m_hWnd);


    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    ImGui::StyleColorsMarkTechDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    ImGui_ImplWin32_Init(m_hWnd);
    io.FontDefault = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\arial.ttf", 18.0f);
}

void CWinWindow::KillWindow()
{
    DestroyWindow(m_hWnd);
    UnregisterClass(m_WndClass.lpszClassName, m_WndClass.hInstance);
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyPlatformWindows();
}

void CWinWindow::MessageLoop(const MSG* pMsg)
{
    TranslateMessage(pMsg);
    DispatchMessageW(pMsg);
    switch (pMsg->message)
    {
    case WM_MOVE:
        m_nX = LOWORD(pMsg->lParam);
        m_nY = HIWORD(pMsg->lParam);
        break;
    case WM_SIZE:
        m_nWidth = LOWORD(pMsg->lParam);
        m_nHeight = HIWORD(pMsg->lParam);
        break;
    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYDOWN:
    case WM_SYSKEYUP:
        bool bDown = (pMsg->lParam & 1 << 31) == 0;
        m_pInputRef->PollKeyboardInput((UINT)pMsg->wParam, bDown);
        break;
    }
}

void CWinWindow::SetHInstance(HINSTANCE hInstance)
{
    m_hInstance = hInstance;
}

extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT WindowProc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, Msg, wParam, lParam))
        return true;

    switch (Msg)
    {
    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }break;
    }
    return DefWindowProc(hWnd, Msg, wParam, lParam);
}
