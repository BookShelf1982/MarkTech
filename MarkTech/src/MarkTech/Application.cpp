#include "Application.h"

#include "MarkTech/Events/ApplicationEvent.h"
#include "MarkTech/Log.h"

namespace MarkTech {

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
	}
}