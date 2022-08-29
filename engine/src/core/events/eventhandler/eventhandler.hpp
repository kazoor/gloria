#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <iterator>
#include <map>
#include "../events.hpp"

namespace gloria::core {
	typedef std::multimap<const std::type_info*,
		const std::function<void(const Event&)>> EventMap;

	class EventHandler {
	private:
		static EventMap eventMap;

	public:
		template<typename EventWanted>
		static void on(const std::function<void(const Event&)>& fn) {
			EventHandler::eventMap.emplace(&typeid(EventWanted), fn);
		}

		static void emit(const Event& event) {
			auto range = eventMap.equal_range(&typeid(event));
			for (auto it = range.first; it != range.second; ++it)
				it->second(event);
		}
	};
}