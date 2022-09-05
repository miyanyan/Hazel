#pragma once

#include <memory>

#include "spdlog/spdlog.h"

#include "Core.h"

namespace Hazel {
	class HAZEL_API Log
	{
	public:
		static Log& getInstance();
		void log();
		std::shared_ptr<spdlog::logger>& getCoreLogger();
		std::shared_ptr<spdlog::logger>& getClientLogger();
	private:
		Log();
		~Log();
		Log(const Log&) = delete;
		Log& operator=(const Log&) = delete;

		std::shared_ptr<spdlog::logger> m_coreLogger;
		std::shared_ptr<spdlog::logger> m_clientLogger;
	};
}

// Core log macros
#define HZ_CORE_TRACE(...)		::Hazel::Log::getInstance().getCoreLogger()->trace(__VA_ARGS__)
#define HZ_CORE_INFO(...)		::Hazel::Log::getInstance().getCoreLogger()->info(__VA_ARGS__)
#define HZ_CORE_WARN(...)		::Hazel::Log::getInstance().getCoreLogger()->warn(__VA_ARGS__)
#define HZ_CORE_ERROR(...)		::Hazel::Log::getInstance().getCoreLogger()->error(__VA_ARGS__)
#define HZ_CORE_CRITICAL(...)   ::Hazel::Log::getInstance().getCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define HZ_TRACE(...)			::Hazel::Log::getInstance().getClientLogger()->trace(__VA_ARGS__)
#define HZ_INFO(...)			::Hazel::Log::getInstance().getClientLogger()->info(__VA_ARGS__)
#define HZ_WARN(...)			::Hazel::Log::getInstance().getClientLogger()->warn(__VA_ARGS__)
#define HZ_ERROR(...)			::Hazel::Log::getInstance().getClientLogger()->error(__VA_ARGS__)
#define HZ_CRITICAL(...)	    ::Hazel::Log::getInstance().getClientLogger()->critical(__VA_ARGS__)
