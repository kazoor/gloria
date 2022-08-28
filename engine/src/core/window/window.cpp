#include "window.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace gloria::core {
	void windowSizeCallback(GLFWwindow* window, int width, int height) {
		GlWindowInfo_t& windowInfo = *reinterpret_cast<GlWindowInfo_t*>(glfwGetWindowUserPointer(window));
		windowInfo.windowSize = { width, height };
		windowInfo.resized = true; // this doesnt get reset atm
	}

	void windowShouldCloseCallback(GLFWwindow* window) {
		GlWindowInfo_t& windowInfo = *reinterpret_cast<GlWindowInfo_t*>(glfwGetWindowUserPointer(window));
		windowInfo.shouldClose = true;
	}

	void windowFrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
		GlWindowInfo_t& windowInfo = *reinterpret_cast<GlWindowInfo_t*>(glfwGetWindowUserPointer(window));
		windowInfo.frameBufferSize = { width, height };
	}

	void windowPositionCallback(GLFWwindow* window, int xpos, int ypos) {
		GlWindowInfo_t& windowInfo = *reinterpret_cast<GlWindowInfo_t*>(glfwGetWindowUserPointer(window));
		windowInfo.windowPos = { xpos, ypos };
	}

	void windowContentScaleCallback(GLFWwindow* window, float xscale, float yscale) {
		GlWindowInfo_t& windowInfo = *reinterpret_cast<GlWindowInfo_t*>(glfwGetWindowUserPointer(window));
		windowInfo.contentScale = { xscale, yscale };
	}

	void windowFocusCallback(GLFWwindow* window, int focused) {
		GlWindowInfo_t& windowInfo = *reinterpret_cast<GlWindowInfo_t*>(glfwGetWindowUserPointer(window));
			windowInfo.focused = static_cast<bool>(focused);
	}

	Window::Window() {

	}

	Window::Window(int width, int height, const std::string& title) {
		mWindow = createWindowPtr(width, height, title);
	}

	Window::~Window() {

	}

	void Window::createWindow(int width, int height, const std::string& title) {
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		// glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		glfwSetWindowSizeCallback(mWindow, windowSizeCallback);

		GlWindowInfo_t info = {
			.windowSize = {width, height},
			.title = title,
			.focused = true,
			.resized = false,
			.shouldClose = false,
			.ctx = mWindow
		};

		mWindowInfo = info;

		glfwSetWindowUserPointer(mWindow, &mWindowInfo);

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

		GlWindowInfo_t info = {
			.windowSize = {width, height},
			.title = title,
			.focused = true,
			.resized = false,
			.shouldClose = false,
			.ctx = window
		};

		mWindowInfo = info;

		glfwSetWindowUserPointer(window, &mWindowInfo);

		// callbacks
		glfwSetWindowSizeCallback(window, windowSizeCallback);

		glfwSetWindowCloseCallback(window, windowShouldCloseCallback);

		glfwSetFramebufferSizeCallback(window, windowFrameBufferSizeCallback);

		glfwSetWindowPosCallback(window, windowPositionCallback);

		glfwSetWindowContentScaleCallback(window, windowContentScaleCallback);

		glfwSetWindowFocusCallback(window, windowFocusCallback);

		return window;
	}

	void Window::pollEvents() {
		while (!glfwWindowShouldClose(mWindow)) {
			glfwPollEvents();
		}
	}

	GLFWwindow* Window::getWindow() {
		return mWindow;
	}

	void Window::destroy() {
		glfwDestroyWindow(mWindow);

		glfwTerminate();
	}

	GlWindowInfo_t Window::getWindowInfo() {
		return mWindowInfo;
	}

	void Window::setTitle(const std::string& title) {
		glfwSetWindowTitle(mWindow, title.c_str());
		mWindowInfo.title = title;
	}
}