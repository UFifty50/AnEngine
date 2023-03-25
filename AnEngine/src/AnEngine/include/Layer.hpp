#ifndef LAYER_HPP
#define LAYER_HPP


#include "Core.hpp"
#include "Events/Event.hpp"

namespace AnEngine {
    class AE_API Layer {
    protected:
        std::string debugName;

    public:
           Layer(const std::string& name = "Layer");
           virtual ~Layer();
    
           virtual void onAttach() {}
           virtual void onDetach() {}
           virtual void onUpdate() {}
           virtual void onEvent(Event& e) {}
    
           inline const std::string& getName() const { return debugName; }
    };
}

#endif
