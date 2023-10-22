#ifndef SCENESPANEL_HPP
#define SCENESPANEL_HPP

#include "Core/Core.hpp"
#include "Panels/Panel.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine::Crank {
    class ScenesPanel : public Panel {
    public:
        ScenesPanel() = default;
        ScenesPanel(std::string name, const Ref<Scene>& scene);

        void setCurrentScene(const Ref<Scene>& scene);
        Entity getSelectedEntity() { return selectedEntity; }

        virtual void beforeRender() override {}
        virtual void render() override;
        virtual void afterRender() override {}

        virtual std::string getName() override { return name; }

    private:
        void drawEntityNode(Entity entity);

        std::string name;
        Ref<Scene> currentScene;
        Entity selectedEntity;
    };
};  // namespace AnEngine::Crank

#endif
