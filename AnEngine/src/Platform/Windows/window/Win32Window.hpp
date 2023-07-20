#ifndef WIN32WINDOW_HPP
#define WIN32WINDOW_HPP

#include <GLFW/glfw3.h>

#include "Renderer/RenderContext.hpp"
#include "Window.hpp"


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
        Ref<RenderContext> graphicsContext;

        virtual void init(const WindowProperties& props);
        virtual void shutdown();

    public:
        Win32Window(const WindowProperties& props);
        virtual ~Win32Window();

        void onUpdate() override;

        inline unsigned int getWidth() const override { return data.width; }
        inline unsigned int getHeight() const override { return data.height; }

        inline void setEventCallback(const EventCallbackFn& callback) override {
            data.eventCallback = callback;
        }
        void setVSync(bool isEnabled) override;
        bool VSyncEnabled() const override;

        inline virtual void* getNativeWindow() const override { return window; }
    };
}  // namespace AnEngine

#endif
