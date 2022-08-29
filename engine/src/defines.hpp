#pragma once
#include <vulkan/vulkan.h>
#include <spdlog/spdlog.h>
#include "utils/logger/logger.hpp"

#ifdef GL_API
#ifdef GL_EXPORT
#define GL_API __declspec(dllexport)
#else
#define GL_API __declspec(dllimport)
#endif
#endif

#define VK_VALIDATE( chck, fmt ) if( chck != VK_SUCCESS ) \
  throw std::runtime_error("Vulkan Error: " fmt )

// Logging
#define GL_CORE_TRACE(...) ::gloria::core::Log::getCoreLogger()->trace(__VA_ARGS__)
#define GL_CORE_INFO(...) ::gloria::core::Log::getCoreLogger()->info(__VA_ARGS__)
#define GL_CORE_WARN(...) ::gloria::core::Log::getCoreLogger()->warn(__VA_ARGS__)
#define GL_CORE_ERROR(...) ::gloria::core::Log::getCoreLogger()->error(__VA_ARGS__)
#define GL_CORE_CRITICAL(...) ::gloria::core::Log::getCoreLogger()->critical(__VA_ARGS__)

#define GL_TRACE(...) ::gloria::core::Log::getClientLogger()->trace(__VA_ARGS__)
#define GL_INFO(...) ::gloria::core::Log::getClientLogger()->info(__VA_ARGS__)
#define GL_WARN(...) ::gloria::core::Log::getClientLogger()->warn(__VA_ARGS__)
#define GL_ERROR(...) ::gloria::core::Log::getClientLogger()->error(__VA_ARGS__)
#define GL_CRITICAL(...) ::gloria::core::Log::getClientLogger()->critical(__VA_ARGS__)