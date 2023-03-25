#ifndef EVENT_HPP
#define EVENT_HPP

#include "aepch.hpp"
#include "Core.hpp"

namespace AnEngine {

    // ----- Events -----
    // AnEngine events are currently blocking, so events are dispatched
    // immediately after occurance
    //
    // TODO: Buffer events in an event bus and process them during
    // a dedicated section of the update stage

    enum class EventType {
        None = 0,
        CloseWindow,
        ResizeWindow,
        FocusWindow,
        UnfocusWindow,
        MoveWindow,

        AppTick,
        AppUpdate,
        AppRender,

        KeyPressed,
        KeyReleased,
        KeyTyped,

        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };


    class AE_API Event {
   //     friend class EventDispatcher;
    public:
        bool handled = false;

        virtual EventType getEventType() const = 0;
        virtual const char* getName() const = 0;
        virtual int getCategoryFlags() const = 0;
        virtual std::string toString() const { return getName(); }

        inline bool isInCategory(EventCategory category) {
            return getCategoryFlags() & category;
        }
    };

    class EventDispatcher {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    private:
        Event& event;

    public:
        EventDispatcher(Event& event) : event(event) { }

        template<typename T>
        bool dispatch(EventFn<T> eventFunc) {
            if (event.getEventType() == T::getStaticType()) {
                event.handled = eventFunc(*(T*)&event);
                return true;
            }
            return false;
        }
    };

    inline std::ostream& operator<<(std::ostream& os, const Event& e) {
        return os << e.toString();
    }
}

#define EVENT_CLASS_TYPE(type) static EventType getStaticType() { return EventType::##type; }               \
                               virtual EventType getEventType() const override { return getStaticType(); }  \
                               virtual const char* getName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) virtual int getCategoryFlags() const override { return category; }

#endif
