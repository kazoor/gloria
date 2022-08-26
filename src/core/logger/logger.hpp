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
		Log() {}
		~Log() { mCoreLogger.reset(); mClientLogger.reset(); }

		static void Init();
		static Ref<spdlog::logger>& getCoreLogger() { return mCoreLogger; }
		static Ref<spdlog::logger>& getClientLogger() { return mClientLogger; }
	private:
		static Ref<spdlog::logger> mCoreLogger;
		static Ref<spdlog::logger> mClientLogger;
	};
}