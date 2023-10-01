#pragma once
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"


class CLog
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr <spdlog::logger> GetClientLogger() { return s_ClientLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_ClientLogger;
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
};

#define	MT_CORE_ERROR(...) CLog::GetCoreLogger()->error(__VA_ARGS__)
#define	MT_CORE_WARN(...) CLog::GetCoreLogger()->warn(__VA_ARGS__)
#define	MT_CORE_INFO(...) CLog::GetCoreLogger()->info(__VA_ARGS__)
#define	MT_CORE_TRACE(...) CLog::GetCoreLogger()->trace(__VA_ARGS__)
#define	MT_CORE_FATAL(...) CLog::GetCoreLogger()->fatal(__VA_ARGS__)

#define	MT_ERROR(...) CLog::GetClientLogger()->error(__VA_ARGS__)
#define	MT_WARN(...) CLog::GetClientLogger()->warn(__VA_ARGS__)
#define	MT_INFO(...) CLog::GetClientLogger()->info(__VA_ARGS__)
#define	MT_TRACE(...) CLog::GetClientLogger()->trace(__VA_ARGS__)
#define	MT_FATAL(...) CLog::GetClientLogger()->fatal(__VA_ARGS__)

