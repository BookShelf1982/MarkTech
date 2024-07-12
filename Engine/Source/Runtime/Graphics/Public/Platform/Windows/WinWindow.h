#pragma once
#include "Window.h"
#include <Windows.h>

namespace MarkTech
{
	class WinWindow : public Window
	{
	public:
		WinWindow(WindowProperties props);
		~WinWindow();

		virtual void SetEventHandler(PF_WINDOWEVENTHANDLER pfn) override;
		virtual void PollWindowEvents() override;
		virtual WindowProperties GetProperties() const override;
		virtual void Destroy() override;

	private:
		HWND m_Window;
		WindowProperties m_Properties;
	};
}