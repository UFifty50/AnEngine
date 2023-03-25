#ifndef WIN32_WINDOW
#define WIN32_WINDOW

#include "Window.hpp"
#include <GLFW/glfw3.h>


namespace AnEngine {
    class Win32Window : public Window {
    private:
        struct WindowData {
            std::string title = "";
            unsigned int width = 0;
            unsigned int height = 0;
            bool vSync = false;
            EventCallbackFn eventCallback = nullptr;
        };

        GLFWwindow* window;
        WindowData data;

        virtual void init(const WindowProperties& props);
        virtual void shutdown();

    public:
        Win32Window(const WindowProperties& props);
        virtual ~Win32Window();

        void onUpdate() override;

        inline unsigned int getWidth() const override { return data.width; }
        inline unsigned int getHeight() const override { return data.height; }

        inline void setEventCallback(const EventCallbackFn& callback) override { data.eventCallback = callback; }
        void setVSync(bool enabled) override;
        bool VSyncEnabled() const override;
    };
}

#endif
