#pragma once
#include "Core.h"
#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class CInput
{
public:
	CInput();
	~CInput();

	bool InitInput(HINSTANCE hInstance, HWND hWnd);
	void PollInput();
	void DestroyInput();

	bool IsKeyDown(uint32_t keycode);
private:
	IDirectInputDevice8* m_pDIKeyboard;
	IDirectInputDevice8* m_pDIMouse;

	DIMOUSESTATE m_MouseLastState;
	DIMOUSESTATE m_MouseCurrentState;
	BYTE m_KeyboardState[256];
	LPDIRECTINPUT8 m_DirectInput;
};

