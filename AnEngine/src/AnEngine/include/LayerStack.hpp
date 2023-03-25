#ifndef LAYERSTACK_HPP
#define LAYERSTACK_HPP


#include <vector>

#include "Core.hpp"
#include "Layer.hpp"

namespace AnEngine {
    class AE_API LayerStack {
    private:
        std::vector<Layer*> layers;
        std::vector<Layer*>::iterator layerInsert;

    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Layer* layer);
        void pushOverlay(Layer* overlay);
        void popLayer(Layer* layer);
        void popOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return layers.begin(); }
        std::vector<Layer*>::iterator end() { return layers.end(); }
    };
}

#endif
