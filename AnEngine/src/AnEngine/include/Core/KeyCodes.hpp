#ifndef KEYCODES_HPP
#define KEYCODES_HPP


namespace AnEngine {
    class KeyCode {
    public:
        enum Code : uint16_t {
            // From glfw3.h
            Space = 32,
            Apostrophe = 39, /* ' */
            Comma = 44,      /* , */
            Minus = 45,      /* - */
            Period = 46,     /* . */
            Slash = 47,      /* / */

            D0 = 48, /* 0 */
            D1 = 49, /* 1 */
            D2 = 50, /* 2 */
            D3 = 51, /* 3 */
            D4 = 52, /* 4 */
            D5 = 53, /* 5 */
            D6 = 54, /* 6 */
            D7 = 55, /* 7 */
            D8 = 56, /* 8 */
            D9 = 57, /* 9 */

            Semicolon = 59, /* ; */
            Equal = 61,     /* = */

            A = 65,
            B = 66,
            C = 67,
            D = 68,
            E = 69,
            F = 70,
            G = 71,
            H = 72,
            I = 73,
            J = 74,
            K = 75,
            L = 76,
            M = 77,
            N = 78,
            O = 79,
            P = 80,
            Q = 81,
            R = 82,
            S = 83,
            T = 84,
            U = 85,
            V = 86,
            W = 87,
            X = 88,
            Y = 89,
            Z = 90,

            LeftBracket = 91,  /* [ */
            Backslash = 92,    /* \ */
            RightBracket = 93, /* ] */
            GraveAccent = 96,  /* ` */

            World1 = 161, /* non-US #1 */
            World2 = 162, /* non-US #2 */

            /* Function keys */
            Escape = 256,
            Enter = 257,
            Tab = 258,
            Backspace = 259,
            Insert = 260,
            Delete = 261,
            Right = 262,
            Left = 263,
            Down = 264,
            Up = 265,
            PageUp = 266,
            PageDown = 267,
            Home = 268,
            End = 269,
            CapsLock = 280,
            ScrollLock = 281,
            NumLock = 282,
            PrintScreen = 283,
            Pause = 284,
            F1 = 290,
            F2 = 291,
            F3 = 292,
            F4 = 293,
            F5 = 294,
            F6 = 295,
            F7 = 296,
            F8 = 297,
            F9 = 298,
            F10 = 299,
            F11 = 300,
            F12 = 301,
            F13 = 302,
            F14 = 303,
            F15 = 304,
            F16 = 305,
            F17 = 306,
            F18 = 307,
            F19 = 308,
            F20 = 309,
            F21 = 310,
            F22 = 311,
            F23 = 312,
            F24 = 313,
            F25 = 314,

            /* Keypad */
            KP0 = 320,
            KP1 = 321,
            KP2 = 322,
            KP3 = 323,
            KP4 = 324,
            KP5 = 325,
            KP6 = 326,
            KP7 = 327,
            KP8 = 328,
            KP9 = 329,
            KPDecimal = 330,
            KPDivide = 331,
            KPMultiply = 332,
            KPSubtract = 333,
            KPAdd = 334,
            KPEnter = 335,
            KPEqual = 336,

            LeftShift = 340,
            LeftControl = 341,
            LeftAlt = 342,
            LeftSuper = 343,
            RightShift = 344,
            RightControl = 345,
            RightAlt = 346,
            RightSuper = 347,
            Menu = 348
        };

        KeyCode() = default;
        constexpr KeyCode(Code code) : code(code) {}

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


// extracted from glfw3.h

/* The unknown key */
#define AE_KEY_UNKNOWN -1

/* Printable keys */
#define AE_KEY_SPACE ::AnEngine::KeyCode::Space
#define AE_KEY_APOSTROPHE ::AnEngine::KeyCode::Apostrophe /* ' */
#define AE_KEY_COMMA ::AnEngine::KeyCode::Comma           /* , */
#define AE_KEY_MINUS ::AnEngine::KeyCode::Minus           /* - */
#define AE_KEY_PERIOD ::AnEngine::KeyCode::Period         /* . */
#define AE_KEY_SLASH ::AnEngine::KeyCode::Slash           /* / */
#define AE_KEY_0 ::AnEngine::KeyCode::D0
#define AE_KEY_1 ::AnEngine::KeyCode::D1
#define AE_KEY_2 ::AnEngine::KeyCode::D2
#define AE_KEY_3 ::AnEngine::KeyCode::D3
#define AE_KEY_4 ::AnEngine::KeyCode::D4
#define AE_KEY_5 ::AnEngine::KeyCode::D5
#define AE_KEY_6 ::AnEngine::KeyCode::D6
#define AE_KEY_7 ::AnEngine::KeyCode::D7
#define AE_KEY_8 ::AnEngine::KeyCode::D8
#define AE_KEY_9 ::AnEngine::KeyCode::D9
#define AE_KEY_SEMICOLON ::AnEngine::KeyCode::Semicolon /* ; */
#define AE_KEY_EQUAL ::AnEngine::KeyCode::Equal         /* = */
#define AE_KEY_A ::AnEngine::KeyCode::A
#define AE_KEY_B ::AnEngine::KeyCode::B
#define AE_KEY_C ::AnEngine::KeyCode::C
#define AE_KEY_D ::AnEngine::KeyCode::D
#define AE_KEY_E ::AnEngine::KeyCode::E
#define AE_KEY_F ::AnEngine::KeyCode::F
#define AE_KEY_G ::AnEngine::KeyCode::G
#define AE_KEY_H ::AnEngine::KeyCode::H
#define AE_KEY_I ::AnEngine::KeyCode::I
#define AE_KEY_J ::AnEngine::KeyCode::J
#define AE_KEY_K ::AnEngine::KeyCode::K
#define AE_KEY_L ::AnEngine::KeyCode::L
#define AE_KEY_M ::AnEngine::KeyCode::M
#define AE_KEY_N ::AnEngine::KeyCode::N
#define AE_KEY_O ::AnEngine::KeyCode::O
#define AE_KEY_P ::AnEngine::KeyCode::P
#define AE_KEY_Q ::AnEngine::KeyCode::Q
#define AE_KEY_R ::AnEngine::KeyCode::R
#define AE_KEY_S ::AnEngine::KeyCode::S
#define AE_KEY_T ::AnEngine::KeyCode::T
#define AE_KEY_U ::AnEngine::KeyCode::U
#define AE_KEY_V ::AnEngine::KeyCode::V
#define AE_KEY_W ::AnEngine::KeyCode::W
#define AE_KEY_X ::AnEngine::KeyCode::X
#define AE_KEY_Y ::AnEngine::KeyCode::Y
#define AE_KEY_Z ::AnEngine::KeyCode::Z
#define AE_KEY_LEFT_BRACKET ::AnEngine::KeyCode::LeftBracket   /* [ */
#define AE_KEY_BACKSLASH ::AnEngine::KeyCode::Backslash        /* \ */
#define AE_KEY_RIGHT_BRACKET ::AnEngine::KeyCode::RightBracket /* ] */
#define AE_KEY_GRAVE_ACCENT ::AnEngine::KeyCode::GraveAccent   /* ` */
#define AE_KEY_WORLD_1 ::AnEngine::KeyCode::World1             /* non-US #1 */
#define AE_KEY_WORLD_2 ::AnEngine::KeyCode::World2             /* non-US #2 */

/* Function keys */
#define AE_KEY_ESCAPE ::AnEngine::KeyCode::Escape
#define AE_KEY_ENTER ::AnEngine::KeyCode::Enter
#define AE_KEY_TAB ::AnEngine::KeyCode::Tab
#define AE_KEY_BACKSPACE ::AnEngine::KeyCode::Backspace
#define AE_KEY_INSERT ::AnEngine::KeyCode::Insert
#define AE_KEY_DELETE ::AnEngine::KeyCode::Delete
#define AE_KEY_RIGHT ::AnEngine::KeyCode::Right
#define AE_KEY_LEFT ::AnEngine::KeyCode::Left
#define AE_KEY_DOWN ::AnEngine::KeyCode::Down
#define AE_KEY_UP ::AnEngine::KeyCode::Up
#define AE_KEY_PAGE_UP ::AnEngine::KeyCode::PageUp
#define AE_KEY_PAGE_DOWN ::AnEngine::KeyCode::PageDown
#define AE_KEY_HOME ::AnEngine::KeyCode::Home
#define AE_KEY_END ::AnEngine::KeyCode::End
#define AE_KEY_CAPS_LOCK ::AnEngine::KeyCode::CapsLock
#define AE_KEY_SCROLL_LOCK ::AnEngine::KeyCode::ScrollLock
#define AE_KEY_NUM_LOCK ::AnEngine::KeyCode::NumLock
#define AE_KEY_PRINT_SCREEN ::AnEngine::KeyCode::PrintScreen
#define AE_KEY_PAUSE ::AnEngine::KeyCode::Pause
#define AE_KEY_F1 ::AnEngine::KeyCode::F1
#define AE_KEY_F2 ::AnEngine::KeyCode::F2
#define AE_KEY_F3 ::AnEngine::KeyCode::F3
#define AE_KEY_F4 ::AnEngine::KeyCode::F4
#define AE_KEY_F5 ::AnEngine::KeyCode::F5
#define AE_KEY_F6 ::AnEngine::KeyCode::F6
#define AE_KEY_F7 ::AnEngine::KeyCode::F7
#define AE_KEY_F8 ::AnEngine::KeyCode::F8
#define AE_KEY_F9 ::AnEngine::KeyCode::F9
#define AE_KEY_F10 ::AnEngine::KeyCode::F10
#define AE_KEY_F11 ::AnEngine::KeyCode::F11
#define AE_KEY_F12 ::AnEngine::KeyCode::F12
#define AE_KEY_F13 ::AnEngine::KeyCode::F13
#define AE_KEY_F14 ::AnEngine::KeyCode::F14
#define AE_KEY_F15 ::AnEngine::KeyCode::F15
#define AE_KEY_F16 ::AnEngine::KeyCode::F16
#define AE_KEY_F17 ::AnEngine::KeyCode::F17
#define AE_KEY_F18 ::AnEngine::KeyCode::F18
#define AE_KEY_F19 ::AnEngine::KeyCode::F19
#define AE_KEY_F20 ::AnEngine::KeyCode::F20
#define AE_KEY_F21 ::AnEngine::KeyCode::F21
#define AE_KEY_F22 ::AnEngine::KeyCode::F22
#define AE_KEY_F23 ::AnEngine::KeyCode::F23
#define AE_KEY_F24 ::AnEngine::KeyCode::F24
#define AE_KEY_F25 ::AnEngine::KeyCode::F25

/* Keypad */
#define AE_KEY_KP_0 ::AnEngine::KeyCode::KP0
#define AE_KEY_KP_1 ::AnEngine::KeyCode::KP1
#define AE_KEY_KP_2 ::AnEngine::KeyCode::KP2
#define AE_KEY_KP_3 ::AnEngine::KeyCode::KP3
#define AE_KEY_KP_4 ::AnEngine::KeyCode::KP4
#define AE_KEY_KP_5 ::AnEngine::KeyCode::KP5
#define AE_KEY_KP_6 ::AnEngine::KeyCode::KP6
#define AE_KEY_KP_7 ::AnEngine::KeyCode::KP7
#define AE_KEY_KP_8 ::AnEngine::KeyCode::KP8
#define AE_KEY_KP_9 ::AnEngine::KeyCode::KP9
#define AE_KEY_KP_DECIMAL ::AnEngine::KeyCode::KPDecimal
#define AE_KEY_KP_DIVIDE ::AnEngine::KeyCode::KPDivide
#define AE_KEY_KP_MULTIPLY ::AnEngine::KeyCode::KPMultiply
#define AE_KEY_KP_SUBTRACT ::AnEngine::KeyCode::KPSubtract
#define AE_KEY_KP_ADD ::AnEngine::KeyCode::KPAdd
#define AE_KEY_KP_ENTER ::AnEngine::KeyCode::KPEnter
#define AE_KEY_KP_EQUAL ::AnEngine::KeyCode::KPEqual

#define AE_KEY_LEFT_SHIFT ::AnEngine::KeyCode::LeftShift
#define AE_KEY_LEFT_CONTROL ::AnEngine::KeyCode::LeftControl
#define AE_KEY_LEFT_ALT ::AnEngine::KeyCode::LeftAlt
#define AE_KEY_LEFT_SUPER ::AnEngine::KeyCode::LeftSuper
#define AE_KEY_RIGHT_SHIFT ::AnEngine::KeyCode::RightShift
#define AE_KEY_RIGHT_CONTROL ::AnEngine::KeyCode::RightControl
#define AE_KEY_RIGHT_ALT ::AnEngine::KeyCode::RightAlt
#define AE_KEY_RIGHT_SUPER ::AnEngine::KeyCode::RightSuper
#define AE_KEY_MENU ::AnEngine::KeyCode::Menu

#define AE_KEY_LAST AE_KEY_MENU

/*! @brief If this bit is set one or more Shift keys were held down.
 *
 *  If this bit is set one or more Shift keys were held down.
 */
#define AE_MOD_SHIFT 0x0001
/*! @brief If this bit is set one or more Control keys were held down.
 *
 *  If this bit is set one or more Control keys were held down.
 */
#define AE_MOD_CONTROL 0x0002
/*! @brief If this bit is set one or more Alt keys were held down.
 *
 *  If this bit is set one or more Alt keys were held down.
 */
#define AE_MOD_ALT 0x0004
/*! @brief If this bit is set one or more Super keys were held down.
 *
 *  If this bit is set one or more Super keys were held down.
 */
#define AE_MOD_SUPER 0x0008
/*! @brief If this bit is set the Caps Lock key is enabled.
 *
 *  If this bit is set the Caps Lock key is enabled and the @ref
 *  AE_LOCK_KEY_MODS input mode is set.
 */
#define AE_MOD_CAPS_LOCK 0x0010
/*! @brief If this bit is set the Num Lock key is enabled.
 *
 *  If this bit is set the Num Lock key is enabled and the @ref
 *  AE_LOCK_KEY_MODS input mode is set.
 */
#define AE_MOD_NUM_LOCK 0x0020

#endif
