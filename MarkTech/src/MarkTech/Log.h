#pragma once

#include <memory>
#include "core.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace MarkTech {

	class MARKTECH_API Log
	{
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};

}

// core log macros

#define MT_CORE_INFO(...)    ::MarkTech::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MT_CORE_TRACE(...)   ::MarkTech::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MT_CORE_WARN(...)    ::MarkTech::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MT_CORE_ERROR(...)   ::MarkTech::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MT_CORE_FATAL(...)   ::MarkTech::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// client log macros
#define MT_INFO(...)         ::MarkTech::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MT_TRACE(...)        ::MarkTech::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MT_WARN(...)         ::MarkTech::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MT_ERROR(...)        ::MarkTech::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MT_FATAL(...)        ::MarkTech::Log::GetCoreLogger()->fatal(__VA_ARGS__)