#include "Application.h"
#include "Events/ApplicationEvent.h"
#include "Log.h"

namespace MarkTech {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		MT_TRACE(e);
		while (true)
		{

		}
	}

}