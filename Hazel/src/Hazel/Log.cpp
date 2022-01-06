#include "Log.h"
#include "spdlog\sinks\stdout_color_sinks.h"
#include <iostream>

namespace Hazel {
	//redefine our logger ptrs
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	// needed to add HAZEL_API_LOG for some reason (assuming the init() function didnt get dllexported along with the class)
	void HAZEL_API Log::Init() {

		//sets our standard logging pattern
		spdlog::set_pattern("%^[%T] %n: %v%$");

		//initialize core logger
		s_CoreLogger = spdlog::stdout_color_mt("Hazel");
		s_CoreLogger->set_level(spdlog::level::trace);


		//initialize client logger
		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);


	}

}
