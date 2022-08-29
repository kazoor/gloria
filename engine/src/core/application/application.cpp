#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include "application.hpp"
#include "../../utils/log/log.hpp"
#include "../../utils/base.hpp"
#include "../instance/instance.hpp"

namespace gloria::core {
	Application::Application() {
		gInstance = std::make_shared<Instance>();
		Log::Init();
		Instance::get().getWindow().createWindow(1920, 1080, "App");
	}

	Application::Application(const std::string& title) {
		gInstance = std::make_shared<Instance>();
		Log::Init();
		Instance::get().getWindow().createWindow(1920, 1080, title);
	}

	Application::~Application() {
		Instance::get().getWindow().destroy();
	}

	void Application::pushLayer(Shared<Layer> layer)
	{
		Instance::get().getWindow().pushLayer(layer);
		layer.get()->onAttach();
	}

	void Application::pushOverlay(Shared<Layer> overlay) {
		Instance::get().getWindow().pushOverlay(overlay);
		overlay.get()->onAttach();
	}

	void Application::run() {
		while (Instance::get().getWindow().isRunning()) {
			Instance::get().getWindow().update();
		}
	}

	Window& Application::getWindow() {
		return Instance::get().getWindow();
	}
}