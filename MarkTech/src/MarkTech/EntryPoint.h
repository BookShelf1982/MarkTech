#pragma once

#ifdef MT_PLATFORM_WINDOWS

extern MarkTech::Application* MarkTech::CreateApplication();

int main(int argc, int** argv)
{
	auto app = MarkTech::CreateApplication();
	app->Run();
	delete app;
}
#endif