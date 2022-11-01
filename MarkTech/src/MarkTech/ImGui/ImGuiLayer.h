#pragma once
#include "MarkTech/core.h"
#include "MarkTech/Layer.h"

#include "MarkTech/Events/MouseEvent.h"
#include "MarkTech/Events/KeyEvent.h"
#include "MarkTech/Events/ApplicationEvent.h"

namespace MarkTech {

	class MARKTECH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		void OnAttach();
		void OnDetach();
		void OnUpdate();
		void OnEvent(Event& event);
	private:
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
		bool OnMouseMovedEvent(MouseMovedEvent& event);
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);
		bool OnKeyPressedEvent(KeyPressedEvent& event);
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);
		bool OnKeyTypedEvent(KeyTypedEvent& event);
		bool OnWindowResizedEvent(WindowResizedEvent& event);

	private:
		float m_Time = 0;

	};

}