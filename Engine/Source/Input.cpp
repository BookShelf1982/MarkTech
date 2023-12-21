#include "Input.h"
namespace MarkTech
{
    bool CInput::IsKeyDown(uint32_t keycode)
    {
        return (m_KeyboardState[keycode] & 0x80);
    }

    bool CInput::IsKeyUp(uint32_t keycode)
    {
        return !(m_KeyboardState[keycode] & 0x80);
    }

    MVector2 CInput::GetMousePos()
    {
        return MVector2((float)m_MouseCurrState.lX, (float)m_MouseCurrState.lY);
    }

    void CInput::PollInput()
    {
        HRESULT hr = DIKeyboard->Acquire();
        assert(SUCCEEDED(hr));
        hr = DIMouse->Acquire();
        assert(SUCCEEDED(hr));

        DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseCurrState);

        DIKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
    }

    bool CInput::InitInput(HINSTANCE hInstance, HWND hwnd)
    {
        HRESULT hr = DirectInput8Create(hInstance, DIRECTINPUT_HEADER_VERSION, IID_IDirectInput8, (void**)&DirectInput, NULL);
        if (FAILED(hr))
            return false;

        hr = DirectInput->CreateDevice(GUID_SysKeyboard,
            &DIKeyboard,
            NULL);
        if (FAILED(hr))
            return false;

        hr = DirectInput->CreateDevice(GUID_SysMouse,
            &DIMouse,
            NULL);
        if (FAILED(hr))
            return false;

        hr = DIKeyboard->SetDataFormat(&c_dfDIKeyboard);
        hr = DIKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

        hr = DIMouse->SetDataFormat(&c_dfDIMouse);
        hr = DIMouse->SetCooperativeLevel(hwnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

        return SUCCEEDED(hr);
    }

    void CInput::DestroyInput()
    {
        DIKeyboard->Unacquire();
        DIMouse->Unacquire();
        DirectInput->Release();
        delete this;
    }

    CInput::CInput()
    {
    }

    CInput::~CInput()
    {
    }
}