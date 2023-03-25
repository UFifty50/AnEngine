#include "aepch.hpp"
#include "LayerStack.hpp"


namespace AnEngine {
    LayerStack::LayerStack() {
        layerInsert = layers.begin();
    }

    LayerStack::~LayerStack() {
        for (Layer* layer : layers) {
        //    layer->onDetach();
            delete layer;
        }
    }

    void LayerStack::pushLayer(Layer* layer) {
        layerInsert = layers.emplace(layerInsert, layer);
    }

    void LayerStack::pushOverlay(Layer* overlay) {
        layers.emplace_back(overlay);
    }

    void LayerStack::popLayer(Layer* layer) {
        auto it = std::ranges::find(layers, layer);
        if (it != layers.end()) {
            layers.erase(it);
            layerInsert--;
        }
    }

    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::ranges::find(layers, overlay);
        if (it != layers.end()) {
            layers.erase(it);
        }
    }
}
