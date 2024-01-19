#ifndef IMGUILAYER_HPP
#define IMGUILAYER_HPP

#include "Core/Layer.hpp"
#include "Events/ApplicationEvent.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Events/MouseEvent.hpp"


namespace AnEngine {
    class AE_API ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        virtual void onAttach() override;
        virtual void onDetach() override;
        virtual void onEvent(Event& event) override;
        virtual void onImGuiRender() override;

        void begin();
        void end();

        void useDarkThemeColours();

        void loadConfig(const std::string& path);
        void saveConfig(const std::string& path) const;

        void shouldAllowEvents(bool allow) { allowEvents = allow; }

    private:
        bool allowEvents = false;
        bool newConfigQueued = false;
        std::string configPath;
    };
}  // namespace AnEngine

#endif
