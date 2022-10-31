#pragma once
#include "MarkTech/core.h"
#include "MarkTech/Layer.h"

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
		float m_Time = 0;

	};

}