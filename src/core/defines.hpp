#pragma once
#include "logger/logger.hpp"

constexpr int MAX_FRAMES_IN_FLIGHT = 2;

#define VK_VALIDATE( chck, fmt ) if( chck != VK_SUCCESS ) \
  throw std::runtime_error("Vulkan Error: " fmt )

inline static const std::vector<const char*> g_deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
};

inline static const std::vector<const char*> g_validationLayers = {
	"VK_LAYER_KHRONOS_validation"
};

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
