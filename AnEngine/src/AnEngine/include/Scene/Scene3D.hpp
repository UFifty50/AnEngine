#ifndef SCENE3D_HPP
#define SCENE3D_HPP

#include <entt/entt.hpp>

#include "Renderer/Camera/EditorCamera.hpp"
#include "Scene/Scene.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Entity;
    struct Component;
    namespace Crank {
        class ScenesPanel;
    };

    class Scene3D : public Scene {
    public:
        Scene3D() = default;
        Scene3D(std::string name) { this->name = name; }
        ~Scene3D() = default;

        void clear() {
            this->name = "";
            entityRegistry.clear();
        }

        virtual bool is3D() const override { return true; }

        virtual Entity& createEntity(const std::string& name = "") override;
        virtual void destroyEntity(Entity& entity) override;
        virtual void onResize(uint32_t width, uint32_t height) override;

        virtual void onUpdateEditor(TimeStep deltaTime,
                                    const Ref<EditorCamera>& camera) override;
        virtual void onUpdateRuntime(TimeStep deltaTime) override;

        Entity getPrimaryCamera();

    private:
        virtual void onComponentAdded(Entity& e, Component& component) override;
    };
}  // namespace AnEngine

#endif
