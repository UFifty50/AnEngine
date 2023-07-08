#ifndef INPUT_HPP
#define INPUT_HPP

#include "Core.hpp"


namespace AnEngine {
    class AE_API Input {
    private:
        static Scope<Input> instance;

    protected:
        virtual bool isKeyPressedImpl(int keycode) = 0;

        virtual bool isMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> getMousePositionImpl() = 0;
        virtual float getMouseXImpl() = 0;
        virtual float getMouseYImpl() = 0;

    public:
        inline static bool isKeyPressed(int keycode) {
            return instance->isKeyPressedImpl(keycode);
        }

        inline static bool isMouseButtonPressed(int button) {
            return instance->isMouseButtonPressedImpl(button);
        }
        inline static std::pair<float, float> getMousePosition() {
            return instance->getMousePositionImpl();
        }
        inline static float getMouseX() { return instance->getMouseXImpl(); }
        inline static float getMouseY() { return instance->getMouseYImpl(); }
    };
}  // namespace AnEngine

#endif
