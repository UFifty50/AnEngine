#ifndef MOUSE_EVENT_HPP
#define MOUSE_EVENT_HPP

#include <sstream>
#include <string>

#include "Core/Core.hpp"
#include "Event.hpp"


namespace AnEngine {
    class MouseMovedEvent : public Event {
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

    class MouseScrolledEvent : public Event {
    private:
        float Xoffset;
        float Yoffset;

    public:
        MouseScrolledEvent(float Xoffset, float Yoffset)
            : Xoffset(Xoffset), Yoffset(Yoffset) {}

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

    class MouseButtonEvent : public Event {
    protected:
        MouseButtonEvent(int button) : button(button) {}
        int button;

    public:
        inline int getMouseButton() const { return button; }

        EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput)
    };

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        MouseButtonPressedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonPressedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonPressed)
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        MouseButtonReleasedEvent(int button) : MouseButtonEvent(button) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "MouseButtonReleasedEvent: " << button;
            return ss.str();
        }

        EVENT_CLASS_TYPE(MouseButtonReleased)
    };
}  // namespace AnEngine

#endif
