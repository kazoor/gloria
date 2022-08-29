#pragma once

namespace gloria::core {

	struct Event {
		virtual ~Event() {}
	};

	struct WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height) : w(width), h(height) {}

		int w{ 0 };
		int h{ 0 };
	};
}