#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"


std::shared_ptr<spdlog::logger> CLog::s_ClientLogger;
std::shared_ptr<spdlog::logger> CLog::s_CoreLogger;

void CLog::Init()
{
	spdlog::set_pattern("[%T] %n: %v%$");
	s_CoreLogger = spdlog::stdout_color_mt("MARKTECH");
	s_CoreLogger->set_level(spdlog::level::trace);

	s_ClientLogger = spdlog::stdout_color_mt("GAME");
	s_ClientLogger->set_level(spdlog::level::trace);
}