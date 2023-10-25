#ifndef SANDBOX2D_HPP
#define SANDBOX2D_HPP

#include <glm/glm.hpp>

#include "../../Crank/src/include/Dockspace.hpp"
#include "Core/Core.hpp"
#include "Core/Layer.hpp"
#include "Events/Event.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"
#include "Texture/Sprite.hpp"
#include "Texture/SpriteSheet.hpp"


namespace AnEngine {
    class SandBox2D : public Layer {
    public:
        SandBox2D();
        virtual ~SandBox2D() = default;

        virtual void onAttach() override;
        virtual void onDetach() override;

        virtual void onUpdate(TimeStep deltaTime) override;
        virtual void onImGuiRender() override;
        virtual void onEvent(Event& event) override;

    private:
        Crank::DockSpace dockSpace;
        Ref<Scene> activeScene;
        Ref<FrameBuffer> frameBuffer;
        SpriteSheet sheet;
        Sprite sprite1;
        Entity playerEntity;
        Entity cameraEntity;
        Entity lockedCameraEntity;

        bool CameraA = true;
    };
}  // namespace AnEngine

/*
class TiledWorld2D {
public:
    TiledWorld2D(const std::vector<std::vector<AnEngine::Sprite>>& tiles);

    void render();
};
*/

#endif
