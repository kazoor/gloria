#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <tuple>

namespace gloria::core {
	struct GlWindowInfo_t {
		std::tuple<int, int> windowSize;
		std::tuple<int, int> frameBufferSize;
		std::tuple<int, int> windowPos;
		std::tuple<float, float> contentScale;
		std::string title;
		bool focused;
		bool resized;
		bool shouldClose;
		GLFWwindow* ctx;
	};

	class Window
	{
	public:
		Window();

		Window(int width, int height, const std::string& title);

		~Window();

		void createWindow(int width, int height, const std::string& title);

		GLFWwindow* createWindowPtr(int width, int height, const std::string& title);

		void pollEvents();

		GLFWwindow* getWindow();

		void destroy();

		GlWindowInfo_t getWindowInfo();

		void setTitle(const std::string& title);
	private:
		GLFWwindow* mWindow{ nullptr };

		GlWindowInfo_t mWindowInfo;
	};
}