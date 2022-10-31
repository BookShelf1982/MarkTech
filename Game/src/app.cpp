#include "MarkTech.h"

class ExampleLayer : public MarkTech::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		MT_INFO("ExampleLayer::Update");
	}

	void OnEvent(MarkTech::Event& event) override
	{
		MT_TRACE("{0}", event);
	}
};


class game : public MarkTech::Application
{
public:
	game()
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new MarkTech::ImGuiLayer());
	}

	~game()
	{

	}
};

MarkTech::Application* MarkTech::CreateApplication()
{
	return new game();
}