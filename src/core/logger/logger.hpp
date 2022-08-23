#pragma once
#include "../base.hpp"
// Ignore all spdlog warnings d:^)
// They dont exist if we cant see them.
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace gloria::core {
	class Log
	{
	public:
		static void Init();
		static Ref<spdlog::logger>& getCoreLogger() { return m_coreLogger; }
		static Ref<spdlog::logger>& getClientLogger() { return m_clientLogger; }
	private:
		static Ref<spdlog::logger> m_coreLogger;
		static Ref<spdlog::logger> m_clientLogger;
	};
}