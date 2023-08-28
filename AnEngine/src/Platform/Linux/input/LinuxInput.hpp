#ifndef LINUXDOWSINPUT_HPP
#define LINUXDOWSINPUT_HPP

#include <glm/glm.hpp>

#include "Core/Input.hpp"


namespace AnEngine {
    class LinuxInput : public Input {
    protected:
        virtual bool isKeyPressedImpl(int keycode) override;

        virtual bool isMouseButtonPressedImpl(int button) override;
        virtual float getMouseXImpl() override;
        virtual float getMouseYImpl() override;
        virtual glm::vec2 getMousePositionImpl() override;
    };
}  // namespace AnEngine

#endif
