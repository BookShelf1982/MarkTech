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

	//TO BE DEFINED IN CLIENT
	Application* CreateApplication();

}

