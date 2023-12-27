#include "Input.h"

CInput::CInput()
	:m_DirectInput(), m_MouseLastState(), m_pDIKeyboard(), m_pDIMouse()
{
}

CInput::~CInput()
{
}

bool CInput::InitInput(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = DirectInput8Create(
		hInstance,
		DIRECTINPUT_HEADER_VERSION,
		IID_IDirectInput8, 
		(void**)&m_DirectInput, 
		NULL);

	if (FAILED(hr))
		return false;

	hr = m_DirectInput->CreateDevice(GUID_SysKeyboard,
		&m_pDIKeyboard,
		NULL);
	if (FAILED(hr))
		return false;

	hr = m_DirectInput->CreateDevice(GUID_SysMouse,
		&m_pDIMouse,
		NULL);
	if (FAILED(hr))
		return false;

	hr = m_pDIKeyboard->SetDataFormat(&c_dfDIKeyboard);
	hr = m_pDIKeyboard->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	hr = m_pDIMouse->SetDataFormat(&c_dfDIMouse);
	hr = m_pDIMouse->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND);

	return true;
}

void CInput::PollInput()
{
	m_pDIKeyboard->Acquire();
	m_pDIMouse->Acquire();

	m_pDIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseCurrentState);
	m_pDIKeyboard->GetDeviceState(sizeof(m_KeyboardState), (LPVOID)&m_KeyboardState);
}

void CInput::DestroyInput()
{
	m_pDIKeyboard->Unacquire();
	m_pDIMouse->Unacquire();
	m_DirectInput->Release();
}

bool CInput::IsKeyDown(uint32_t keycode)
{
	if (m_KeyboardState[keycode] & 0x80)
		return true;
	else
		return false;
}
