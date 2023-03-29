#ifndef WIN32WINDOW_HPP
#define WIN32WINDOW_HPP

#include "Window.hpp"
#include "Renderer/RenderContext.hpp"
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
        RenderContext* graphicsContext;

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

        inline virtual void* getNativeWindow() const override { return window; }
    };
}

#endif
