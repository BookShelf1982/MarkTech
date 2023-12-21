#pragma once
#include "Core.h"
#include "Keycodes.h"
#include "Vectors.h"

#define DIRECTINPUT_VERSION 0x0800
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#include <dinput.h>

namespace MarkTech
{
	class CInput
	{
	public:
		CInput();
		~CInput();

		bool IsKeyDown(uint32_t keycode);
		bool IsKeyUp(uint32_t keycode);

		bool IsButtonDown(uint32_t buttoncode);
		bool IsButtonUp(uint32_t buttoncode);

		MVector2 GetMousePos();

		void PollInput();

		bool InitInput(HINSTANCE hInstance, HWND hwnd);

		void DestroyInput();

	private:
		IDirectInputDevice8* DIKeyboard = NULL;
		IDirectInputDevice8* DIMouse = NULL;
		LPDIRECTINPUT8 DirectInput = NULL;

		DIMOUSESTATE m_MouseCurrState;
		BYTE m_KeyboardState[256];
	};
}