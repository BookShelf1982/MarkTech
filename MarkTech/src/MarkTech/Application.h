#pragma once
#include "core.h"
#include "Events/Event.h"


namespace MarkTech {

	class MARKTECH_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	Application* CreateApplication();
 }

