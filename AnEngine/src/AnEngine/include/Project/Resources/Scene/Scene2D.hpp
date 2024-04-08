#ifndef SCENE2D_HPP
#define SCENE2D_HPP

#define NOMINMAX
#include <entt/entt.hpp>

#include "Core/UUID.hpp"
#include "Project/Resource.hpp"
#include "Project/Resources/Scene/Scene.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Entity;
    struct Component;
    namespace Crank {
        class ScenesPanel;
    };

    class Scene2D : public Scene, public Resource {
    public:
        ~Scene2D() = default;

        static Resource create(const std::string& name = "") {
            return static_cast<Resource>(Scene2D(name));
        }

        void empty() {
            Resource::name = "";
            entityRegistry.clear();
        }

        virtual bool is3D() const override { return false; }

        virtual const std::string& getName() const override { return Resource::name; }
        virtual void setName(const std::string& newName) override { Resource::name = newName; }

        virtual Entity& createEntity(const std::string& name = "") override;
        virtual Entity& createEntityWithUUID(const std::string& name, UUID id) override;
        virtual void destroyEntity(Entity& entity) override;
        virtual void onResize(uint32_t width, uint32_t height) override;

        virtual void onUpdateEditor(TimeStep deltaTime,
                                    const Ref<EditorCamera>& camera) override;
        virtual void onUpdateRuntime(TimeStep deltaTime) override;

        Entity getPrimaryCamera();

    private:
        Scene2D(const std::string& name = "") : Resource{Resource::Type::Scene2D, {}, name} {}

        virtual void onComponentAdded(Entity& e, Component& component) override;
    };
}  // namespace AnEngine

#endif
