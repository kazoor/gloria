#pragma once
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
namespace gloria::core {
	class Window {
	public:
		Window(std::uint32_t width, std::uint32_t height, const std::string& title) : m_width(width), m_height(height), m_title(title) {
			createWindow();
		}

		std::uint32_t getWidth() const {
			return m_width;
		}

		std::uint32_t getHeight() const {
			return m_height;
		}

		std::string getTitle() const {
			return m_title;
		}

		GLFWwindow* getWindowPtr() const {
			return m_window;
		}

	private:
		void createWindow() {
			glfwInit();

			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
			glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

			m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);
		}

	private:
		std::uint32_t m_width, m_height;
		std::string m_title;
		GLFWwindow* m_window;
	};
}