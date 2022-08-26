#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../defines.hpp"
#include "../../window/window.hpp"

namespace gloria::core {
	class WindowSurface {
	public:
		WindowSurface();

		WindowSurface(VkInstance instance, Window window);

		~WindowSurface();

		void createWindowSurface(VkInstance instance, Window window);

		void destroy(VkInstance instance);

		VkSurfaceKHR getSurface();

	private:
		VkSurfaceKHR mSurface;
	};
}