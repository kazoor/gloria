#include "surface.hpp"
#include <vulkan/vulkan.h>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../defines.hpp"
#include "../../core/instance/instance.hpp"

namespace gloria::vk {
	Surface::Surface() {
	}

	Surface::~Surface() {
	}

	void Surface::init() {
		createSurface();
	}

	void Surface::createSurface() {
		VK_VALIDATE(glfwCreateWindowSurface(core::Instance::get().getVkInstance().get(), core::Instance::get().getWindow().getRawWindow(), nullptr, &mSurface), "Failed to create surface");

#ifdef DEBUG
		if (mSurface != VK_NULL_HANDLE)
			GL_CORE_INFO("Surface created");
#endif // DEBUG
	}

	void Surface::destroy() {
		vkDestroySurfaceKHR(core::Instance::get().getVkInstance().get(), mSurface, nullptr);
	}

	VkSurfaceKHR& Surface::get() {
		return mSurface;
	}
}