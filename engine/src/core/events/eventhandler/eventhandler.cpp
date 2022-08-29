#include "eventhandler.hpp"
#include <iostream>
#include <functional>
#include <vector>

namespace gloria::core {
	template <typename T>
	static void EventHandler::AddListener(std::function<void(T&)> callback) {
		getListeners<T>().push_back(std::move(callback));
	}

	template <typename T>
	void EventHandler::TriggerEvent(T& event) {
		for (auto& listener : getListeners<T>()) {
			listener(event);
		}
	}

	template <typename T>
	std::vector<std::function<void(T&)>>& EventHandler::getListeners() {
		static std::vector<std::function<void(T&)>> listeners;
		return listeners;
	}
}