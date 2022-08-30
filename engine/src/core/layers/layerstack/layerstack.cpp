#include "layerstack.hpp"
#include <vector>
#include <string>
#include "../layer/layer.hpp"
#include "../../../utils/base.hpp"

namespace gloria::core {
	LayerStack::LayerStack() {
		mLayerInsert = mLayers.begin();
	}

	LayerStack::~LayerStack() {
	}

	void LayerStack::pushLayer(Shared<Layer> layer) {
		mLayerInsert = mLayers.emplace(mLayerInsert, layer);
	}

	void LayerStack::pushOverlay(Shared<Layer> overlay) {
		mLayers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Shared<Layer> layer) {
		auto it = std::find(mLayers.begin(), mLayers.end(), layer);
		if (it != mLayers.end()) {
			layer.get()->onDetach();
			mLayers.erase(it);
		}
	}

	void LayerStack::popOverlay(Shared<Layer> overlay) {
		auto it = std::find(mLayers.begin(), mLayers.end(), overlay);
		if (it != mLayers.end()) {
			overlay.get()->onDetach();
			mLayers.erase(it);
		}
	}
}