#include "MarkTech.h"

class Game : public MarkTech::Application
{
public:
	Game()
	{

	}
	~Game()
	{

	}
};

MarkTech::Application* MarkTech::CreateApplication()
{
	return new Game();
}
