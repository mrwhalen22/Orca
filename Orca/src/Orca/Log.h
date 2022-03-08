#pragma once
#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

namespace Orca {
	
	class Log {
	public:
		static void Init();

		//returns the shared_ptr values from spdlog for each logger 
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;

	};
}

//Core logging macros
#define OA_CORE_ERROR(...)   ::Orca::Log::GetCoreLogger()->error(__VA_ARGS__)
#define OA_CORE_WARN(...)    ::Orca::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define OA_CORE_INFO(...)    ::Orca::Log::GetCoreLogger()->info(__VA_ARGS__)
#define OA_CORE_TRACE(...)   ::Orca::Log::GetCoreLogger()->trace(__VA_ARGS__)

//Client logging macros
#define OA_ERROR(...) ::Orca::Log::GetClientLogger()->error(__VA_ARGS__)
#define OA_WARN(...)  ::Orca::Log::GetClientLogger()->warn(__VA_ARGS__)
#define OA_INFO(...)  ::Orca::Log::GetClientLogger()->info(__VA_ARGS__)
#define OA_TRACE(...) ::Orca::Log::GetClientLogger()->trace(__VA_ARGS__)


