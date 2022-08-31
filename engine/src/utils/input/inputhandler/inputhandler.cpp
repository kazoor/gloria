#include "inputhandler.hpp"
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "../../../defines.hpp"
#include "../../../core/instance/instance.hpp"

double m_ScrollX = 0.0;
double m_ScrollY = 0.0;

namespace gloria::util {
	bool InputHandler::isKeyPressedImpl(int KeyCode) {
		auto State = glfwGetKey(core::Instance::get().getWindow().getRawWindow(), KeyCode);

		return State == GLFW_PRESS || State == GLFW_REPEAT;
	}

	bool InputHandler::isMouseKeyPressedImpl(int KeyCode) {
		auto State = glfwGetMouseButton(core::Instance::get().getWindow().getRawWindow(), KeyCode);

		return State == GLFW_PRESS;
	}

	std::pair<double, double> InputHandler::getMousePositionImpl() {
		double X, Y;
		glfwGetCursorPos(core::Instance::get().getWindow().getRawWindow(), &X, &Y);

		return { X, Y };
	}

	double InputHandler::getMouseXImpl() {
		auto [X, Y] = getMousePositionImpl();

		return X;
	}

	double InputHandler::getMouseYImpl() {
		auto [X, Y] = getMousePositionImpl();

		return Y;
	}

	double InputHandler::getScrollXImpl() {
		auto [X, Y] = getScrollInputImpl();

		return X;
	}

	double InputHandler::getScrollYImpl() {
		auto [X, Y] = getScrollInputImpl();

		return Y;
	}

	void InputHandler::resetScrollImpl() { // Reset each frame.
		m_ScrollY = 0.0;
		m_ScrollY = 0.0;
	}

	void scrollCallback(GLFWwindow* wnd, double offsetx, double offsety) {
		m_ScrollX = offsetx;
		m_ScrollY = offsety;
		GL_CORE_INFO("Detected scroll: {0}, {1}", offsetx, offsety);
	}

	std::pair<double, double> InputHandler::getScrollInputImpl() {
		glfwSetScrollCallback(core::Instance::get().getWindow().getRawWindow(), scrollCallback);
		return { m_ScrollX, m_ScrollY };
	}
}