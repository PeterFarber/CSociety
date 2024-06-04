#pragma once

#include <Society/Core/Base.h>

#include <spdlog/spdlog.h>

namespace sc {
	class DECLSPEC Log {
		public:
			static void Init();

			inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() {
				return s_CoreLogger;
			}

			inline static std::shared_ptr<spdlog::logger> &GetClientLogger() {
				return s_ClientLogger;
			}

		private:
			static std::shared_ptr<spdlog::logger> s_CoreLogger;
			static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

// Core log macros
#define SC_CORE_TRACE(...) ::sc::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define SC_CORE_INFO(...) ::sc::Log::GetCoreLogger()->info(__VA_ARGS__)
#define SC_CORE_WARN(...) ::sc::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define SC_CORE_ERROR(...) ::sc::Log::GetCoreLogger()->error(__VA_ARGS__)
#define SC_CORE_FATAL(...) ::sc::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define SC_TRACE(...) ::sc::Log::GetClientLogger()->trace(__VA_ARGS__)
#define SC_INFO(...) ::sc::Log::GetClientLogger()->info(__VA_ARGS__)
#define SC_WARN(...) ::sc::Log::GetClientLogger()->warn(__VA_ARGS__)
#define SC_ERROR(...) ::sc::Log::GetClientLogger()->error(__VA_ARGS__)
#define SC_FATAL(...) ::sc::Log::GetClientLogger()->fatal(__VA_ARGS__)

