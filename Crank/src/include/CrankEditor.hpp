#ifndef CRANKEDITOR_HPP
#define CRANKEDITOR_HPP

#include <glm/glm.hpp>

#include "Core/Core.hpp"
#include "Core/Layer.hpp"
#include "Dockspace.hpp"
#include "Events/Event.hpp"
#include "Events/KeyEvent.hpp"
#include "Menus/Filemenu.hpp"
#include "Panels/Panel.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
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

        bool onKeyPressed(KeyPressedEvent& kpEvent);

        friend class Panel;

    private:
        Ref<Scene> activeScene;
        Ref<FrameBuffer> frameBuffer;

        Ref<EditorCamera> editorCam;

        Ref<DockSpace> dockSpace;

        Ref<ScenesPanel> sceneHierarchy;
        Ref<PropertiesPanel> properties;
        Ref<ViewportPanel> viewport;
        Ref<StatisticsPanel> statistics;

        Ref<FileMenu> fileMenu;

        bool CameraA = true;
    };
}  // namespace AnEngine::Crank

#endif
