#ifndef IMGUILAYER_HPP
#define IMGUILAYER_HPP

#include "AnEngine/include/Core.hpp"
#include "AnEngine/include/Layer.hpp"
#include "AnEngine/include/Events/Event.hpp"


namespace AnEngine {
    class AE_API ImGuiLayer : public Layer {
    private:
        float prevTime = 0.0f;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void onAttach();
        void onDetach();
        void onUpdate();
        void onEvent(Event& e);
    };
}

#endif