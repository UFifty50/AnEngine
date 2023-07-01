#include "aepch.hpp"
#include "LayerStack.hpp"


namespace AnEngine {
    LayerStack::LayerStack() {}

    LayerStack::~LayerStack() {
        for (Layer* layer : layers) {
        //    layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        layers.emplace(layers.begin() + layerInsertIndex, layer);
        layerInsertIndex++;
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::ranges::find(layers, layer);
        if (it != layers.end()) {
            layer->onDetach();
            layers.erase(it);
            layerInsertIndex--;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::ranges::find(layers, overlay);
        if (it != layers.end()) {
            overlay->onDetach();
            layers.erase(it);
        }
    }
}
