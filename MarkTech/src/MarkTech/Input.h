#pragma once

#include "MarkTech/core.h"

namespace MarkTech {

	class MARKTECH_API Input
	{
	public:
		inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImp1(keycode); }
		
		inline static bool IsMouseButtonPressed(int keycode) { return s_Instance->IsMouseButtonPressedImp1(keycode); }
		inline static std::pair<float, float> GetMousePos() { return s_Instance->GetMousePosImp1(); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImp1(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImp1(); }

	protected:
		virtual bool IsKeyPressedImp1(int keycode) = 0;
		virtual bool IsMouseButtonPressedImp1(int button) = 0;
		virtual std::pair<float, float> GetMousePosImp1() = 0;
		virtual float GetMouseXImp1() = 0;
		virtual float GetMouseYImp1() = 0;
	private:
		static Input* s_Instance;
	};

}
