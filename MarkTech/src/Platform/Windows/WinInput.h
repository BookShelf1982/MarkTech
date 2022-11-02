#pragma once
#include "MarkTech/Input.h"

namespace MarkTech {

	class WinInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImp1(int keycode) override;

		virtual bool IsMouseButtonPressedImp1(int keycode) override;
		virtual std::pair<float, float> GetMousePosImp1() override;
		virtual float GetMouseXImp1() override;
		virtual float GetMouseYImp1() override;
	};

}
