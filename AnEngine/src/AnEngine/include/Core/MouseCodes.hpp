#ifndef MOUSECODES_HPP
#define MOUSECODES_HPP

#include <cstdint>


namespace AnEngine {
    class MouseCode {
    public:
        enum Code : uint16_t {
            // From glfw3.h
            Button0 = 0,
            Button1 = 1,
            Button2 = 2,
            Button3 = 3,
            Button4 = 4,
            Button5 = 5,
            Button6 = 6,
            Button7 = 7,

            ButtonLast = Button7,
            ButtonLeft = Button0,
            ButtonRight = Button1,
            ButtonMiddle = Button2
        };

        MouseCode() = default;
        constexpr MouseCode(Code code) : code(code) {}

        constexpr operator Code() const { return code; }
        explicit constexpr operator bool() const = delete;
        constexpr bool operator==(Code other) const { return this->code == other; }
        constexpr bool operator!=(Code other) const { return this->code != other; }
        constexpr uint16_t asInt() { return (uint16_t)this->code; }

        static uint16_t asInt(Code code) { return (uint16_t)code; }

    private:
        Code code;
    };
}  // namespace AnEngine

#define AE_MOUSE_BUTTON_0 ::AnEngine::MouseCode::Button0
#define AE_MOUSE_BUTTON_1 ::AnEngine::MouseCode::Button1
#define AE_MOUSE_BUTTON_2 ::AnEngine::MouseCode::Button2
#define AE_MOUSE_BUTTON_3 ::AnEngine::MouseCode::Button3
#define AE_MOUSE_BUTTON_4 ::AnEngine::MouseCode::Button4
#define AE_MOUSE_BUTTON_5 ::AnEngine::MouseCode::Button5
#define AE_MOUSE_BUTTON_6 ::AnEngine::MouseCode::Button6
#define AE_MOUSE_BUTTON_7 ::AnEngine::MouseCode::Button7
#define AE_MOUSE_BUTTON_LAST ::AnEngine::MouseCode::ButtonLast
#define AE_MOUSE_BUTTON_LEFT ::AnEngine::MouseCode::ButtonLeft
#define AE_MOUSE_BUTTON_RIGHT ::AnEngine::MouseCode::ButtonRight
#define AE_MOUSE_BUTTON_MIDDLE ::AnEngine::MouseCode::ButtonMiddle

#endif
