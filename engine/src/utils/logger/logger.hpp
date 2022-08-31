#pragma once
#include "../base.hpp"
// Ignore all spdlog warnings d:^)
// They dont exist if we cant see them.
#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace gloria::core {
class Log {
   public:
    Log() {}
    ~Log() {
        mCoreLogger.reset();
        mClientLogger.reset();
    }

    static void Init();
    static Shared<spdlog::logger>& getCoreLogger() { return mCoreLogger; }
    static Shared<spdlog::logger>& getClientLogger() { return mClientLogger; }

   private:
    static Shared<spdlog::logger> mCoreLogger;
    static Shared<spdlog::logger> mClientLogger;
};
}  // namespace gloria::core