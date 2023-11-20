#pragma once
#include "Core.h"
#include "Keycodes.h"
#include <Windows.h>
namespace MarkTech
{
	struct MKeyState
	{
		MKeyState(bool isDown = false, bool wasDown = false) : IsDown(isDown), WasDown(wasDown) {}

		bool IsDown, WasDown;
	};

	struct MKeyboardInput
	{
		MKeyboardInput() {}
		MKeyState keys[MTVK_MAX_KEYS];
	};

	class CInput
	{
	public:
		bool IsKeyDown(uint32_t keycode);
		bool IsKeyUp(uint32_t keycode);

		/**DO NOT USE THS FOR GAMEPLAY!* Use GetState(), IsKeyDown() IsKeyUp() instead. */
		void PollInput(uint32_t keycode, bool IsDown, bool WasDown);

		MKeyState GetKeyState(uint32_t keycode);

		static CInput* GetInput() { return Input; }

	private:
		CInput() {}

		static CInput* Input;

		static MKeyboardInput keyboard;

	};
}