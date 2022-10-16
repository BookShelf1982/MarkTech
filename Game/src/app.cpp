#include "MarkTech.h"

class game : public MarkTech::Application
{
public:
	game()
	{

	}

	~game()
	{

	}
};

MarkTech::Application* MarkTech::CreateApplication()
{
	return new game();
}