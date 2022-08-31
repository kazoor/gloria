#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>
#include <tuple>
#include "../../utils/base.hpp"
#include "../../utils/time/time.hpp"
#include "../layers/layerstack/layerstack.hpp"

namespace gloria::core {
	class Window
	{
	public:
		Window();

		Window(int width, int height, const std::string& title);

		~Window();

		void createWindow(int width, int height, const std::string& title);

		GLFWwindow* createWindowPtr(int width, int height, const std::string& title);

		void update();
		
		void destroy();

		void pushLayer(Shared<Layer> layer);

		void pushOverlay(Shared<Layer> overlay);

		GLFWwindow* getRawWindow();

		void setTitle(const std::string& title);

		bool isValid();

		bool isRunning();

	private:
		GLFWwindow* mWindow{ nullptr };
		Unique<LayerStack> mLayerStack;
	};
}