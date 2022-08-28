#pragma once
#include <vulkan/vulkan.h>
#include <iostream>
#include "../../defines.hpp"
#include "../../utils/base.hpp"
#include "../instance/instance.hpp"

namespace gloria::core {
	class Application {
	public:
		Application();

		virtual ~Application();

		virtual void init();

		virtual void update();

		virtual void destroy();

		Instance& getInstance();

	private:
		Shared<Instance> applicationInstance;
	};
}