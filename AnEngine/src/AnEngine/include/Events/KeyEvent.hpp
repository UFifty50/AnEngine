#ifndef KEY_EVENT_HPP
#define KEY_EVENT_HPP

#include <sstream>
#include <string>

#include "Core/Core.hpp"
#include "Event.hpp"


namespace AnEngine {
    class AE_API KeyEvent : public Event {
    protected:
        KeyEvent(int keyCode) : keyCode(keyCode) {}
        int keyCode;

    public:
        inline int getKeyCode() const { return keyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput)
    };

    class AE_API KeyPressedEvent : public KeyEvent {
    private:
        int repeatCount;

    public:
        KeyPressedEvent(int keyCode, int repeatCount)
            : KeyEvent(keyCode), repeatCount(repeatCount) {}

        inline int getRepeatCount() const { return repeatCount; }

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode << " (" << repeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)
    };

    class AE_API KeyReleasedEvent : public KeyEvent {
    public:
        KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class AE_API KeyTypedEvent : public KeyEvent {
    public:
        KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}


        std::string toString() const override {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << keyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}  // namespace AnEngine

#endif
