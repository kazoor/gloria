#include "windowsurface.hpp"

namespace gloria::core {
	WindowSurface::WindowSurface() {}

	WindowSurface::WindowSurface(VkInstance instance, Window window) {
		createWindowSurface(instance, window);
	}
	WindowSurface::~WindowSurface() {}

	void WindowSurface::createWindowSurface(VkInstance instance, Window window) {
		VK_VALIDATE(glfwCreateWindowSurface(instance, window.getWindowPtr(), nullptr, &mSurface), "Failed to create a window surface");

#ifdef DEBUG
		if (mSurface != VK_NULL_HANDLE)
			GL_CORE_INFO("Window surface has been created");
#endif // DEBUG
	}

	void WindowSurface::destroy(VkInstance instance) {
		vkDestroySurfaceKHR(instance, mSurface, nullptr);
	}

	VkSurfaceKHR WindowSurface::getSurface() {
		return mSurface;
	}
}