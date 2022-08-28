#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "application.hpp"
#include "../../utils/log/log.hpp"
#include "../../utils/base.hpp"
#include "../instance/instance.hpp"

namespace gloria::core {
	Application::Application() {
		gInstance = std::make_shared<Instance>();
		applicationInstance = gInstance;
		Log::Init();
	}

	Application::~Application() {}

	void Application::init() {
	}

	void Application::update() {
		GL_CORE_INFO("LMAO");
	}

	void Application::destroy() {
	}

	Instance& Application::getInstance() {
		return *applicationInstance;
	}
}