#pragma once
#include "Core.h"
#include "Keycodes.h"

class CInput
{
public:
	CInput();
	~CInput();

	bool InitInput();
	void PollKeyboardInput(uint32_t nKeycode, bool bIsPressed);
	void PollMouseInout();
	void DestroyInput();

	bool IsKeyDown(uint32_t keycode);
private:
	bool m_bKeyboard[MTKI_MAX_KEYS];
};

