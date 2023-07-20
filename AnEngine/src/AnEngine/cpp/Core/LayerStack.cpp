#include "aepch.hpp"

#include "Core/LayerStack.hpp"


namespace AnEngine {
    LayerStack::LayerStack() {}

    LayerStack::~LayerStack() {
        for (Ref<Layer> layer : layers) {
            layer->onDetach();
            layer->~Layer();
        }
    }

    void LayerStack::pushLayer(Ref<Layer> layer) {
        layers.emplace(layers.begin() + layerInsertIndex, layer);
        layerInsertIndex++;
    }

    void LayerStack::pushOverlay(Ref<Layer> overlay) { layers.emplace_back(overlay); }

    void LayerStack::popLayer(Ref<Layer> layer) {
        auto it = std::ranges::find(layers, layer);
        if (it != layers.end()) {
            layer->onDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Ref<Layer> overlay) {
        auto it = std::ranges::find(layers, overlay);
        if (it != layers.end()) {
            overlay->onDetach();
            layers.erase(it);
        }
    }
}  // namespace AnEngine
