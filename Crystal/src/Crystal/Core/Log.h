#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"


namespace Crystal 
{
	class Log /* log class */
	{
	public:
		static void Init(); /* a static function to initialize the logger */

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }		/* a static function that returns a shared pointer reference to the core logger */
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; } /* a static function that returns a shared pointer reference to the client logger */
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;	/* the core logger */
		static std::shared_ptr<spdlog::logger> s_ClientLogger;	/* the client logger */
	};
}

//Core Log Macros
#define CRYSTAL_CORE_FATAL(...) ::Crystal::Log::GetCoreLogger()->critical(__VA_ARGS__)
#define CRYSTAL_CORE_ERROR(...) ::Crystal::Log::GetCoreLogger()->error(__VA_ARGS__)
#define CRYSTAL_CORE_WARN(...)  ::Crystal::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CRYSTAL_CORE_INFO(...)  ::Crystal::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CRYSTAL_CORE_TRACE(...) ::Crystal::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client Log Macros
#define CRYSTAL_FATAL(...)		::Crystal::Log::GetClientLogger()->critical(__VA_ARGS__)
#define CRYSTAL_ERROR(...)		::Crystal::Log::GetClientLogger()->error(__VA_ARGS__)
#define CRYSTAL_WARN(...)		::Crystal::Log::GetClientLogger()->warn(__VA_ARGS__)
#define CRYSTAL_INFO(...)		::Crystal::Log::GetClientLogger()->info(__VA_ARGS__)
#define CRYSTAL_TRACE(...)		::Crystal::Log::GetClientLogger()->trace(__VA_ARGS__)
