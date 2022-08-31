#pragma once
#include <string>
#include "../../../utils/time/time.hpp"

namespace gloria::core {
	class Layer {
	public:
		Layer(const std::string &layerName) : mLayerName(layerName) {}

		virtual ~Layer() {}

		virtual void onAttach() {}
		
		virtual void onDetach() {}

		virtual void onUpdate(util::Time t) {}

		std::string getLayername() const { return mLayerName;  }

	private:
		std::string mLayerName{ "" };
	};
}