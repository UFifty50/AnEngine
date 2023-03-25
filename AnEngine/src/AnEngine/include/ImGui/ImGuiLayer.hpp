#ifndef IMGUILAYER_HPP
#define IMGUILAYER_HPP

#include "Layer.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"


namespace AnEngine {
    class AE_API ImGuiLayer : public Layer {
    private:
        bool onMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool onMouseMovedEvent(MouseMovedEvent& e);
        bool onMouseScrolledEvent(MouseScrolledEvent& e);
        bool onKeyPressedEvent(KeyPressedEvent& e);
        bool onKeyReleasedEvent(KeyReleasedEvent& e);
        bool onKeyTypedEvent(KeyTypedEvent& e);
        bool onWindowResizeEvent(WindowResizeEvent& e);

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