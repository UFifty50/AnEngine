#ifndef IMGUILAYER_HPP
#define IMGUILAYER_HPP

#include "Core/Layer.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"


namespace AnEngine {
    class AE_API ImGuiLayer : public Layer {
    private:
        float prevTime = 0.0f;

    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onImGuiRender() override;

        void begin();
        void end();
    };
}  // namespace AnEngine

#endif
