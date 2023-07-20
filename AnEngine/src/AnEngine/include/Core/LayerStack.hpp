#ifndef LAYERSTACK_HPP
#define LAYERSTACK_HPP


#include <vector>

#include "Core/Core.hpp"
#include "Layer.hpp"

namespace AnEngine {
    class AE_API LayerStack {
    private:
        std::vector<Ref<Layer>> layers;
        unsigned int layerInsertIndex = 0;

    public:
        LayerStack();
        ~LayerStack();

        void pushLayer(Ref<Layer> layer);
        void pushOverlay(Ref<Layer> overlay);
        void popLayer(Ref<Layer> layer);
        void popOverlay(Ref<Layer> overlay);

        std::vector<Ref<Layer>>::iterator begin() { return layers.begin(); }
        std::vector<Ref<Layer>>::iterator end() { return layers.end(); }
    };
}  // namespace AnEngine

#endif
