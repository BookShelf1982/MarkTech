#pragma once

#include "Log.h"

#ifdef  MT_PLATFORM_WINDOWS

extern MarkTech::Application* MarkTech::CreateApplication();

int main(int argc, char** argv)
{
	MarkTech::Log::Init();
	MT_CORE_WARN("fuck you");

	auto app = MarkTech::CreateApplication();
	app->Run();
	delete app;
}

#endif //  MT_PLATFORM_WINDOWS

