#include "Input.h"
#include <PoolAllocator.h>
#ifdef MT_PLATFORM_WINDOWS
#include <Windows.h>
#endif


namespace MarkTech
{
    struct MouseState
    {
        I32 x, y;
        U8 mouseButtons;
    };

    bool keyState[104] = {};
    MouseState mouseState = {};

    struct KeyBinding
    {
        U8 id;
        Keycode keycode;
        KeyBinding* pNext;
    };

    KeyBinding* pFirstBinding = nullptr;

    PoolAllocator gBindingAlloc = {};

    bool InitInputSystem()
    {
        gBindingAlloc = CreatePoolAllocator(sizeof(KeyBinding), 128);
        return true;
    }

    void ShutdownInputSystem()
    {
        FreePoolAllocator(&gBindingAlloc);
    }

    void UpdateKeyboardState(Keycode keycode, bool state)
    {
        keyState[(U32)keycode] = state;
    }

    void UpdateMousePos(I32 x, I32 y)
    {
        mouseState.x = x;
        mouseState.y = y;
    }

    void UpdateMouseButtons(U8 buttons, bool state)
    {
        if (state)
            mouseState.mouseButtons |= buttons;
        else
            mouseState.mouseButtons &= ~buttons;
    }

    Keycode ConvertWin32KeycodeToMarkTechKeycode(U64 keycode)
    {
        switch (keycode)
        {
        case VK_ESCAPE:
            return Keycode::ESCAPE;
        default:
            return Keycode::ZERO;
        }
    }

    void BindKey(Keycode keycode, U8 bindId)
    {
        KeyBinding* pBinding = (KeyBinding*)AllocFromPool(&gBindingAlloc);
        pBinding->keycode = keycode;
        pBinding->id = bindId;
        pBinding->pNext = nullptr;

        if (!pFirstBinding)
        {
            pFirstBinding = pBinding;
            return;
        }

        KeyBinding* pLastBinding = pFirstBinding;
        while (pFirstBinding->pNext != nullptr)
        {
            pLastBinding = pLastBinding->pNext;
        }

        pLastBinding->pNext = pBinding;
    }

    void UnbindKey(U8 bindId)
    {
    }

    bool GetBindState(U8 bindId)
    {
        KeyBinding* pBinding = pFirstBinding;
        while (pFirstBinding != nullptr)
        {
            if (pBinding->id == bindId)
            {
                return keyState[(U32)pBinding->keycode];
            }
            pBinding = pBinding->pNext;
        }

        return false;
    }

    bool GetMouseButton(MouseButtons button)
    {
        return ((mouseState.mouseButtons & button) != 0);
    }
}