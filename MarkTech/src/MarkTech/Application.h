#pragma once
#include "core.h"


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

