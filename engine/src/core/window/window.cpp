#include "window.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <memory>
#include "../../defines.hpp"
#include "../../utils/base.hpp"
#include "../../utils/time/time.hpp"
#include "../instance/instance.hpp"

namespace gloria::core {
	static float lastFrame = 0.0f;

	void windowSizeCallback(GLFWwindow* window, int width, int height) {

	}

	void windowShouldCloseCallback(GLFWwindow* window) {

	}

	void windowFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {

	}

	void windowPositionCallback(GLFWwindow* window, int xpos, int ypos) {

	}

	void windowContentScaleCallback(GLFWwindow* window, float xscale, float yscale) {

	}

	void windowFocusCallback(GLFWwindow* window, int focused) {

	}

	Window::Window() {
		mLayerStack = std::make_unique<LayerStack>();
	}

	Window::Window(int width, int height, const std::string& title) {
		mWindow = createWindowPtr(width, height, title);
		mLayerStack = std::make_unique<LayerStack>();
	}

	Window::~Window() {
	}

	void Window::createWindow(int width, int height, const std::string& title) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

#ifdef DEBUG
		if (isValid())
			GL_CORE_INFO("Window created");
		else
			GL_CORE_ERROR("Failed to create window");
#endif // DEBUG

		glfwSetWindowSizeCallback(mWindow, windowSizeCallback);

		// callbacks
		glfwSetWindowSizeCallback(mWindow, windowSizeCallback);

		glfwSetWindowCloseCallback(mWindow, windowShouldCloseCallback);

		glfwSetFramebufferSizeCallback(mWindow, windowFrameBufferSizeCallback);

		glfwSetWindowPosCallback(mWindow, windowPositionCallback);

		glfwSetWindowContentScaleCallback(mWindow, windowContentScaleCallback);

		glfwSetWindowFocusCallback(mWindow, windowFocusCallback);
	}

	GLFWwindow* Window::createWindowPtr(int width, int height, const std::string& title) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		auto window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

#ifdef DEBUG
		if (isValid())
			GL_CORE_INFO("Window created");
		else
			GL_CORE_ERROR("Failed to create window");
#endif // DEBUG

		// callbacks
		glfwSetWindowSizeCallback(window, windowSizeCallback);

		glfwSetWindowCloseCallback(window, windowShouldCloseCallback);

		glfwSetFramebufferSizeCallback(window, windowFrameBufferSizeCallback);

		glfwSetWindowPosCallback(window, windowPositionCallback);

		glfwSetWindowContentScaleCallback(window, windowContentScaleCallback);

		glfwSetWindowFocusCallback(window, windowFocusCallback);

		return window;
	}

	void Window::update() {
		float currentFrame = static_cast<float>(glfwGetTime());
		util::Time t = currentFrame - lastFrame;
		lastFrame = currentFrame;

		for (auto& layer : *mLayerStack) {
			layer->onUpdate(t);
		}

		glfwPollEvents();
	}

	GLFWwindow* Window::getRawWindow() {
		return mWindow;
	}

	void Window::destroy() {
		glfwDestroyWindow(mWindow);

		glfwTerminate();
	}

	void Window::pushLayer(Shared<Layer> layer)
	{
		mLayerStack.get()->pushLayer(layer);
	}

	void Window::pushOverlay(Shared<Layer> overlay)
	{
		mLayerStack.get()->pushOverlay(overlay);
	}

	void Window::setTitle(const std::string& title) {
		glfwSetWindowTitle(mWindow, title.c_str());
	}

	bool Window::isValid() {
		return mWindow != nullptr;
	}

	bool Window::isRunning() {
		return !glfwWindowShouldClose(mWindow);
	}
}