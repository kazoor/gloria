#pragma once
#include <vulkan/vulkan.h>

namespace gloria::vk {
	class Surface {
	public:
		Surface();

		~Surface();

		void init();

		void createSurface();

		void destroy();

		VkSurfaceKHR& get();

	private:
		VkSurfaceKHR mSurface;
	};
}