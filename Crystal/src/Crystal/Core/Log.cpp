#include "crystalpch.h"
#include "Log.h"

namespace Crystal {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;			 /* a shared pointer to a spdlog logger for the core */
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;		 /* a shared pointer to a spdlog logger for the client */

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");					 /* setting the logger's pattern */
		s_CoreLogger = spdlog::stdout_color_mt("Crystal");		 /* setup the core logger color */
		s_CoreLogger->set_level(spdlog::level::trace);			 /* set the logging level to trace by default */

		s_ClientLogger = spdlog::stdout_color_mt("APP");		 /* set the client's logger color */
		s_ClientLogger->set_level(spdlog::level::trace);		 /* set the logging level to trace by default */
	}
}
