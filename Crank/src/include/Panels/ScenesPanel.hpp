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

        virtual void beforeRender() override {
            ImGui::SetNextWindowSizeConstraints({200.0f, -1}, {INFINITY, -1});
        }
        virtual void render() override;
        virtual void afterRender() override {}

        virtual void onClose() override {}

        virtual std::string getName() override { return name; }

    private:
        void drawEntityNode(Entity entity);
        void drawAddEntityUI();

        std::string name;
        Ref<Scene> currentScene;
        Entity selectedEntity;

        // AddEntityUI stuff
        bool isCamera = false;
        bool isSprite = false;
        bool isScripted = false;
        std::string script;
        std::string tagName;
        glm::vec3 transformPos{0.0f};
        glm::vec3 transformRot{0.0f};
        glm::vec3 transformScale{1.0f};
        CameraComponent cc{};
        SpriteRendererComponent sRC{};
        bool entityUIOpen = false;
    };
};  // namespace AnEngine::Crank

#endif
