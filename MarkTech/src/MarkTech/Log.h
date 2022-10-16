#pragma once
#include "core.h"
#include "spdlog/spdlog.h"


namespace MarkTech {

	class MARKTECH_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger> GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr <spdlog::logger> GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
	};

}

#define	MT_CORE_ERROR(...) ::MarkTech::Log::GetCoreLogger()->error(__VA_ARGS__)
#define	MT_CORE_WARN(...) ::MarkTech::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define	MT_CORE_INFO(...) ::MarkTech::Log::GetCoreLogger()->info(__VA_ARGS__)
#define	MT_CORE_TRACE(...) ::MarkTech::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define	MT_CORE_FATAL(...) ::MarkTech::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define	MT_ERROR(...) ::MarkTech::Log::GetClientLogger()->error(__VA_ARGS__)
#define	MT_WARN(...) ::MarkTech::Log::GetClientLogger()->warn(__VA_ARGS__)
#define	MT_INFO(...) ::MarkTech::Log::GetClientLogger()->info(__VA_ARGS__)
#define	MT_TRACE(...) ::MarkTech::Log::GetClientLogger()->trace(__VA_ARGS__)
#define	MT_FATAL(...) ::MarkTech::Log::GetClientLogger()->fatal(__VA_ARGS__)