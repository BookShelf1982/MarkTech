#include "Input.h"

CInput* CInput::Input = new CInput();

MKeyboardInput CInput::keyboard;

bool CInput::IsKeyDown(uint32_t keycode)
{
    return keyboard.keys[keycode].IsDown;
}

bool CInput::IsKeyUp(uint32_t keycode)
{
    return !keyboard.keys[keycode].IsDown;
}

MKeyState CInput::GetKeyState(uint32_t keycode)
{
    return keyboard.keys[keycode];
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
        else if(keycode == VK_SPACE)
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
