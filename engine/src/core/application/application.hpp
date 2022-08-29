#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include <string>
#include "../../defines.hpp"
#include "../../utils/base.hpp"
#include "../instance/instance.hpp"
#include "../layers/layer/layer.hpp"

namespace gloria::core {
	class Application {
	public:
		Application();

		Application(const std::string &title);

		virtual ~Application();

		void pushLayer(Shared<Layer> layer);

		void pushOverlay(Shared<Layer> layer);

		void run();

		void onEvent();

		// virtual void applyLayers(void* ctx) = 0;

		Window& getWindow();
	};
}