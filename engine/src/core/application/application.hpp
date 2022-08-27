#pragma once
#include <iostream>
#include "../../defines.hpp"

namespace gloria::core {
	class Application {
	public:
		Application();
		virtual ~Application();

		virtual void init();

		virtual void update();

		virtual void destroy();
	};
}