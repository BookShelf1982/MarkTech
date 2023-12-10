#include "Input.h"
namespace MarkTech
{
    CInput* CInput::Input = new CInput();

    MKeyboardInput CInput::keyboard;

    MMouseInput CInput::mouse;

    bool CInput::IsKeyDown(uint32_t keycode)
    {
        return keyboard.keys[keycode].IsDown;
    }

    bool CInput::IsKeyUp(uint32_t keycode)
    {
        return !keyboard.keys[keycode].IsDown;
    }

    MButtonState CInput::GetKeyState(uint32_t keycode)
    {
        return keyboard.keys[keycode];
    }

    bool CInput::IsButtonDown(uint32_t buttoncode)
    {
        return mouse.buttons[buttoncode].IsDown;
    }

    bool CInput::IsButtonUp(uint32_t buttoncode)
    {
        return !mouse.buttons[buttoncode].IsDown;
    }

    MVector2 CInput::GetMousePos()
    {
        return MVector2((float)mouse.x, (float)mouse.y);
    }

    void CInput::PollInput(uint32_t keycode, const bool IsDown, const bool WasDown)
    {
        if (WasDown != IsDown)
        {
            if (keycode >= 'A' && keycode <= 'Z')
            {
                uint32_t newKeycode = keycode - 'A';
                keyboard.keys[newKeycode].IsDown = IsDown;
                keyboard.keys[newKeycode].WasDown = WasDown;
            }
            else if (keycode == VK_SPACE)
            {
                keyboard.keys[MTVK_Space].IsDown = IsDown;
                keyboard.keys[MTVK_Space].WasDown = WasDown;
            }
            else if (keycode == VK_UP)
            {
                keyboard.keys[MTVK_Up].IsDown = IsDown;
                keyboard.keys[MTVK_Up].WasDown = WasDown;
            }
            else if (keycode == VK_DOWN)
            {
                keyboard.keys[MTVK_Down].IsDown = IsDown;
                keyboard.keys[MTVK_Down].WasDown = WasDown;
            }
            else if (keycode == VK_LEFT)
            {
                keyboard.keys[MTVK_Left].IsDown = IsDown;
                keyboard.keys[MTVK_Left].WasDown = WasDown;
            }
            else if (keycode == VK_RIGHT)
            {
                keyboard.keys[MTVK_Right].IsDown = IsDown;
                keyboard.keys[MTVK_Right].WasDown = WasDown;
            }
            else if (keycode == VK_ESCAPE)
            {
                keyboard.keys[MTVK_Escape].IsDown = IsDown;
                keyboard.keys[MTVK_Escape].WasDown = WasDown;
            }
        }
    }

    void CInput::PollMouseInput(WPARAM wParam)
    {
        mouse.buttons[MTVM_Mouse1].WasDown = mouse.buttons[MTVM_Mouse1].IsDown;
        mouse.buttons[MTVM_Mouse2].WasDown = mouse.buttons[MTVM_Mouse2].IsDown;
        mouse.buttons[MTVM_Mouse3].WasDown = mouse.buttons[MTVM_Mouse3].IsDown;
        mouse.buttons[MTVM_MouseX1].WasDown = mouse.buttons[MTVM_MouseX1].IsDown;
        mouse.buttons[MTVM_MouseX2].WasDown = mouse.buttons[MTVM_MouseX2].IsDown;

        mouse.buttons[MTVM_Mouse1].IsDown = wParam & MK_LBUTTON;
        mouse.buttons[MTVM_Mouse2].IsDown = wParam & MK_RBUTTON;
        mouse.buttons[MTVM_Mouse3].IsDown = wParam & MK_MBUTTON;
        mouse.buttons[MTVM_MouseX1].IsDown = wParam & MK_XBUTTON1;
        mouse.buttons[MTVM_MouseX2].IsDown = wParam & MK_XBUTTON2;
    }

    void CInput::PollMousePos(int x, int y)
    {
        mouse.x = x;
        mouse.y = y;
    }

    void CInput::DestroyInput()
    {
        delete Input;
    }
}