#ifndef CRANKEDITOR_HPP
#define CRANKEDITOR_HPP

#include <glm/glm.hpp>

#include "Core/Core.hpp"
#include "Core/Layer.hpp"
#include "Dockspace.hpp"
#include "Events/Event.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine::Crank {
    class CrankEditor : public Layer {
    public:
        CrankEditor();
        virtual ~CrankEditor() = default;

        virtual void onAttach() override;
        virtual void onDetach() override;

        virtual void onUpdate(TimeStep deltaTime) override;
        virtual void onImGuiRender() override;
        virtual void onEvent(Event& event) override;

    private:
        DockSpace dockSpace;
        Ref<Scene> activeScene;
        Ref<FrameBuffer> frameBuffer;
        Entity playerEntity;
        Entity cameraEntity;
        Entity lockedCameraEntity;

        bool CameraA = true;
    };
}  // namespace AnEngine::Crank

#endif
