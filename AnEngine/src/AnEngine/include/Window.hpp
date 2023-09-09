#ifndef WINDOW_HPP
#define WINDOW_HPP


#include "aepch.hpp"

#include "Core/Core.hpp"
#include "Events/Event.hpp"

namespace AnEngine {
    struct WindowProperties {
        std::string title;
        uint32_t width;
        uint32_t height;

        WindowProperties(const std::string& title = "AnEngine", uint32_t width = 1280,
                         uint32_t height = 720)
            : title(title), width(width), height(height) {}
    };

    // desktop only; platform independant
    class AE_API Window {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void onUpdate() = 0;

        virtual uint32_t getWidth() const = 0;
        virtual uint32_t getHeight() const = 0;

        virtual void setEventCallback(const EventCallbackFn& callback) = 0;
        virtual void setVSync(bool enabled) = 0;
        virtual bool VSyncEnabled() const = 0;

        virtual void* getNativeWindow() const = 0;

        // platform specific
        static Window* create(const WindowProperties& props = WindowProperties());
    };
}  // namespace AnEngine

#endif
