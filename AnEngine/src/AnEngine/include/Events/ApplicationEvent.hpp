#ifndef APPLICATION_EVENT_HPP
#define APPLICATION_EVENT_HPP

#include "aepch.hpp"
#include "Event.hpp"

namespace AnEngine {
    class AE_API WindowResizeEvent : public Event {
    private:
        unsigned int width, height;

    public:
        WindowResizeEvent(unsigned int width, unsigned int height) : width(width), height(height) {}

        inline unsigned int getWidth() const { return width; }
        inline unsigned int getHeight() const { return height; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "WindowResizeEvent: " << width << ", " << height;
            return ss.str();
        }

        EVENT_CLASS_TYPE(ResizeWindow)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AE_API WindowCloseEvent : public Event {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(CloseWindow)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AE_API AppTickEvent : public Event {
    public:
        AppTickEvent() {}

        EVENT_CLASS_TYPE(AppTick)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AE_API AppUpdateEvent : public Event {
        AppUpdateEvent() {}

        EVENT_CLASS_TYPE(AppUpdate)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class AE_API AppRenderEvent : public Event {
        AppRenderEvent() {}

        EVENT_CLASS_TYPE(AppRender)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };
}

#endif
