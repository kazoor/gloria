#pragma once

namespace gloria::core {
	struct Event {
	public:
		enum class EventType {
			base, // initial

			// window events
			windowClose,
			windowResize,
			windowFocus,
		};

		EventType type = EventType::base;

		Event(EventType type) : type(type) {}
	};
}