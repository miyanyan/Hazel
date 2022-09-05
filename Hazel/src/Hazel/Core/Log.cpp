#include "Log.h"

#include "spdlog/sinks/stdout_color_sinks.h"

namespace Hazel {

	Log::Log() 
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");
		m_coreLogger = spdlog::stdout_color_mt("HAZEL");
		m_coreLogger->set_level(spdlog::level::trace);

		m_clientLogger = spdlog::stdout_color_mt("APP");
		m_clientLogger->set_level(spdlog::level::trace);
	}

	Log::~Log()
	{

	}

	Log& Log::getInstance()
	{
		static Log log;
		return log;
	}

	std::shared_ptr<spdlog::logger>& Log::getCoreLogger()
	{
		return m_coreLogger;
	}

	std::shared_ptr<spdlog::logger>& Log::getClientLogger()
	{
		return m_clientLogger;
	}

}