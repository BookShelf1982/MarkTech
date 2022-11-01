#include "MarkTech.h"

class game : public MarkTech::Application
{
public:
	game()
	{
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