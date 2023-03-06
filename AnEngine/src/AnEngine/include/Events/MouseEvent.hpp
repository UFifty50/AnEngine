#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP


#include "aepch.hpp"
#include "Event.hpp"

namespace AnEngine {
    class AE_API MouseMovedEvent : public Event {
    private:
        float mouseX, mouseY;

    public:
        MouseMovedEvent(float x, float y) : mouseX(x), mouseY(y) {}

        inline float getX() const { return mouseX; }
        inline float getY() const { return mouseY; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseMoved)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class AE_API MouseScrolledEvent : public Event {
    private:
        float Yoffset, Xoffset;

    public:
        MouseScrolledEvent(float  Yoffset, float Xoffset) : Xoffset(Xoffset), Yoffset(Yoffset) {}

        inline float getXOffset() const { return Xoffset; }
        inline float getYOffset() const { return Yoffset; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseScrolledEvent: " << Xoffset << ", " << Yoffset;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseScrolled)
        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class AE_API MouseButtonEvent : public Event {
    protected:
        MouseButtonEvent(int button) : button(button) {}
        int button;

    public:
        inline int getMouseButton() const { return button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class AE_API MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class AE_API MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}

#endif
