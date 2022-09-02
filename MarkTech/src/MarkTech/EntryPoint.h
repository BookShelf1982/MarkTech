#pragma once

#ifdef MT_PLATFORM_WINDOWS

extern MarkTech::Application* MarkTech::CreateApplication();

int main(int argc, int** argv)
{
	MarkTech::Log::Init();
	MT_CORE_INFO("Initialized Logger!");
	MT_INFO("Initialized Client Logger!");
	int a = 1 + 1;
	MT_CORE_INFO("1 + 1 = {0}", a);

	auto app = MarkTech::CreateApplication();
	app->Run();
	delete app;
}
#endif