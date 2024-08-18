#pragma once
#include "Keycodes.h"

namespace MarkTech
{
	enum MouseButtons : U8
	{
		LEFT = 0b1,
		MIDDLE = 0b10,
		RIGHT = 0b100,
		X1 = 0b1000,
		X2 = 0b10000
	};

	bool InitInputSystem();
	void ShutdownInputSystem();
	void UpdateKeyboardState(Keycode keycode, bool state);
	void UpdateMousePos(I32 x, I32 y);
	void UpdateMouseButtons(U8 buttons, bool state);

	Keycode ConvertWin32KeycodeToMarkTechKeycode(U64 keycode);

	void BindKey(Keycode keycode, U8 bindId);
	void UnbindKey(U8 bindId);
	bool GetBindState(U8 bindId);

	bool GetMouseButton(MouseButtons button);
}