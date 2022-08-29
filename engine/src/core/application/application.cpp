#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include "application.hpp"
#include "../../utils/log/log.hpp"
#include "../../utils/base.hpp"
#include "../instance/instance.hpp"
#include "../events/events.hpp"
#include "../events/eventhandler/eventhandler.hpp"
#include "../../utils/input/input.hpp"
#include "../../utils/input/inputhandler/inputhandler.hpp"
#include "../../utils/input/keycodes.hpp"

namespace gloria::core {
	Application::Application() {
		gInstance = std::make_shared<Instance>();
		Log::Init();
		Instance::get().getWindow().createWindow(1920, 1080, "App");
		onEvent();
	}

	Application::Application(const std::string& title) {
		gInstance = std::make_shared<Instance>();
		Log::Init();
		Instance::get().getWindow().createWindow(1920, 1080, title);
		onEvent();
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

	void Application::onEvent() {
		EventHandler::on<WindowResizeEvent>([](const Event& e) {
			auto event = static_cast<const WindowResizeEvent&>(e);
			GL_CORE_INFO("Window resize event triggered! new size: {0}x{1}", event.w, event.h);
		});

		EventHandler::on<WindowFocusEvent>([](const Event& e) {
			auto event = static_cast<const WindowFocusEvent&>(e);
			GL_CORE_INFO("Focus event triggered! focus: {0}", event.focus);
		});
	}

	Window& Application::getWindow() {
		return Instance::get().getWindow();
	}
}