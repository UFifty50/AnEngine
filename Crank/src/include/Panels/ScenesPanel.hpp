#ifndef SCENESPANEL_HPP
#define SCENESPANEL_HPP

#include "Core/Core.hpp"
#include "Panels/Panel.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine::Crank {
    class ScenesPanel : public Panel {
    public:
        ScenesPanel() = default;
        ScenesPanel(std::string name, const Ref<Scene>& scene);

        void setCurrentScene(const Ref<Scene>& scene);
        Entity getSelectedEntity() { return selectedEntity; }

        virtual ImGuiWindowFlags beforeRender() override {
            //   ImGui::SetNextWindowSizeConstraints({200.0f, 0}, {INFINITY, INFINITY});
            return 0;
        }

        virtual void render() override;
        virtual void afterRender() override {}

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

    private:
        void drawEntityNode(Entity entity);
        void drawAddEntityUI();

        friend class ViewportPanel;

        std::string name;
        Ref<Scene> currentScene;
        Entity selectedEntity;

        // AddEntityUI stuff
        bool isCamera = false;
        bool isSprite = false;
        bool isScripted = false;
        std::string script;
        std::string tagName;
        glm::vec3 transformPos{};
        glm::vec3 transformRot{};
        glm::vec3 transformScale{};
        CameraComponent cc;
        SpriteRendererComponent sRC;
        bool entityUIOpen = false;
    };
};  // namespace AnEngine::Crank

#endif
