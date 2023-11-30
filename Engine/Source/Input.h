#pragma once
#include "Core.h"
#include "Keycodes.h"
#include "Vectors.h"
#include <Windows.h>
namespace MarkTech
{
	struct MButtonState
	{
		MButtonState(bool isDown = false, bool wasDown = false) : IsDown(isDown), WasDown(wasDown) {}

		bool IsDown, WasDown;
	};

	struct MKeyboardInput
	{
		MKeyboardInput() {}
		MButtonState keys[MTVK_MAX_KEYS];
	};

	struct MMouseInput
	{
		MMouseInput() : x(0), y(0) {}
		MButtonState buttons[MTVM_MAX_BUTTONS];
		int x;
		int y;
	};


	class CInput
	{
	public:
		bool IsKeyDown(uint32_t keycode);
		bool IsKeyUp(uint32_t keycode);

		MButtonState GetKeyState(uint32_t keycode);

		bool IsButtonDown(uint32_t buttoncode);
		bool IsButtonUp(uint32_t buttoncode);

		MVector2 GetMousePos();

		static CInput* GetInput() { return Input; }

		/**DO NOT USE THS FOR GAMEPLAY!* Use GetState(), IsKeyDown() IsKeyUp() instead. */
		void PollInput(uint32_t keycode, bool IsDown, bool WasDown);

		void PollMouseInput(WPARAM wParam);

		void PollMousePos(int x, int y);

	private:
		CInput() {}

		static CInput* Input;

		static MKeyboardInput keyboard;

		static MMouseInput mouse;

	};
}