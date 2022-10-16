#include "Application.h"

#include "MarkTech/Events/ApplicationEvent.h"
#include "MarkTech/Log.h"

namespace MarkTech {

	Application::Application()
	{

	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		WindowResizedEvent event(1280, 720);
		MT_TRACE(event);

		while (true)
		{

		}
	}
}