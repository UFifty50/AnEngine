#ifndef WINDOW_HPP
#define WINDOW_HPP


#include "aepch.hpp"
#include "Core.hpp"
#include "Events/Event.hpp"

namespace AnEngine {
    struct WindowProperties {
        std::string title;
        unsigned int width;
        unsigned int height;

        WindowProperties(const std::string& title = "AnEngine",
                         unsigned int width = 1280,
                         unsigned int height = 720
                         )
            : title(title), width(width), height(height) {}
    };

    // desktop only; platform independant
    class AE_API Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() {}

        virtual void onUpdate() = 0;

        virtual unsigned int getWidth() const = 0;
        virtual unsigned int getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool VSyncEnabled() const = 0;

        virtual void* getNativeWindow() const = 0;

        // platform specific
        static Window* create(const WindowProperties& props = WindowProperties());
    };
}

#endif
