#include "MarkTech.h"

class GameLayer : public MarkTech::Layer
{
public:
	GameLayer()
		:Layer("Game")
	{
	}

	void OnUpdate() override
	{
		if (MarkTech::Input::IsKeyPressed(MT_KEY_SPACE))
			MT_TRACE("SPACE IS PRESSED!");
	}
};


class game : public MarkTech::Application
{
public:
	game()
	{
		PushLayer(new GameLayer());
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