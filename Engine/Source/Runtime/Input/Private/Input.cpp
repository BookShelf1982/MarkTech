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
		return true;
	}

	void ShutdownInputSystem()
	{
		FreePoolAllocator(gBindingAlloc);
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
		case VK_CONTROL:
			return Keycode::LEFT_CONTROL;
		case 'X':
			return Keycode::X;
		default:
			return Keycode::ZERO;
		}
	}

	bool GetMouseButton(MouseButtons button)
	{
		return ((mouseState.mouseButtons & button) != 0);
	}
}