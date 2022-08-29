#pragma once
#include <iostream>
#include <functional>
#include <vector>

namespace gloria::core {
	class EventHandler
	{
	public:

		template <typename T>
		static void AddListener(std::function<void(T&)> callback);

		template <typename T>
		static void TriggerEvent(T& event);

	private:

		template <typename T>
		static std::vector<std::function<void(T&)>>& getListeners();
	};
}