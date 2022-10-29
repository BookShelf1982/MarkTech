#pragma once
#include "core.h"

#include "Window.h"
#include "LayerStack.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"


namespace MarkTech {

	class MARKTECH_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& event);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
	private:
		bool OnWindowClosed(WindowCloseEvent& event);

		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack;
	};

	Application* CreateApplication();
 }

