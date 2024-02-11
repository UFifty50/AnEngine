#ifndef SCENE2D_HPP
#define SCENE2D_HPP

#define NOMINMAX
#include <entt/entt.hpp>

#include "Core/Core.hpp"
#include "Core/UUID.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Scene/Scene.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Entity;
    struct Component;
    namespace Crank {
        class ScenesPanel;
    };

    class Scene2D : public Scene {
    public:
        Scene2D() = default;
        Scene2D(std::string name) { this->name = name; }
        ~Scene2D() = default;

        void clear() {
            this->name = "";
            entityRegistry.clear();
        }

        virtual bool is3D() const override { return false; }

        virtual Entity& createEntity(const std::string& name = "") override;
        virtual Entity& createEntityWithUUID(const std::string& name, UUID id) override;
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
