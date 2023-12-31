#include "Input.h"

CInput::CInput()
{
}

CInput::~CInput()
{
}

bool CInput::InitInput()
{
	memset(&m_bKeyboard, false, MTKI_MAX_KEYS);
	return true;
}

void CInput::PollKeyboardInput(uint32_t nKeycode, bool bIsPressed)
{
	if(nKeycode < MTKI_MAX_KEYS)
		m_bKeyboard[nKeycode] = bIsPressed;
}

void CInput::DestroyInput()
{
	
}

bool CInput::IsKeyDown(uint32_t nKeycode)
{
	if(nKeycode < MTKI_MAX_KEYS)
		return m_bKeyboard[nKeycode];

	return false;
}
