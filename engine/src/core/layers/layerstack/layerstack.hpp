#pragma once
#include <vector>
#include <string>
#include "../layer/layer.hpp"
#include "../../../utils/base.hpp"

namespace gloria::core {
	class LayerStack {
	public:
		LayerStack();

		~LayerStack();

		void pushLayer(Shared<Layer> layer);

		void pushOverlay(Shared<Layer> layer);

		void popLayer(Shared<Layer> layer);

		void popOverlay(Shared<Layer> layer);

		std::vector<Shared<Layer>>::iterator begin() { return mLayers.begin(); }
		std::vector<Shared<Layer>>::iterator end() { return mLayers.end(); }

		std::vector<Shared<Layer>>::const_iterator begin() const { return mLayers.begin(); }
		std::vector<Shared<Layer>>::const_iterator end() const { return mLayers.end(); }

		std::vector<Shared<Layer>>::reverse_iterator rbegin() { return mLayers.rbegin(); };
		std::vector<Shared<Layer>>::reverse_iterator rend() { return mLayers.rend(); };

		std::vector<Shared<Layer>>::const_reverse_iterator rbegin() const { return mLayers.rbegin(); };
		std::vector<Shared<Layer>>::const_reverse_iterator rend() const { return mLayers.rend(); };

		std::size_t getSize() {
			return mLayers.size();
		}

	private:
		std::vector<Shared<Layer>> mLayers;
		std::vector<Shared<Layer>>::iterator mLayerInsert;
	};
}