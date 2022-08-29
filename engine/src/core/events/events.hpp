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

	struct WindowFrameBufferResizeEvent : public Event {
		WindowFrameBufferResizeEvent(int width, int height) : w(width), h(height) {}

		int w{ 0 };
		int h{ 0 };
	};

	struct WindowPositionEvent : public Event {
		WindowPositionEvent(int xPos, int yPos) : xPos(xPos), yPos(yPos) {}

		int xPos{ 0 };
		int yPos{ 0 };
	};

	struct WindowContentScaleEvent : public Event {
		WindowContentScaleEvent(float xScale, float yScale) : xScale(xScale), yScale(yScale) {}

		float xScale{ 0.0f };
		float yScale{ 0.0f };
	};

	struct WindowFocusEvent : public Event {
		WindowFocusEvent(bool focus) : focus(focus) {}

		bool focus{ true };
	};

	struct WindowShouldCloseEvent : public Event {
		WindowShouldCloseEvent(bool shouldClose) : shouldClose(shouldClose) {}

		bool shouldClose{ false };
	};
}