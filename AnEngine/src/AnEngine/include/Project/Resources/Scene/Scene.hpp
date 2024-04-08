#ifndef SCENE_HPP
#define SCENE_HPP

#define NOMINMAX
#include <entt/entt.hpp>

#include <string>

#include "Core/Core.hpp"
#include "Project/Resource.hpp"
#include "Renderer/Camera/EditorCamera.hpp"


namespace AnEngine {
    class Entity;
    struct Component;

    class Scene {
    public:
        Scene() = default;
        virtual ~Scene() = default;

        virtual bool is3D() const = 0;

        entt::registry& getRegistry() { return entityRegistry; }

        virtual const std::string& getName() const = 0;
        virtual void setName(const std::string& newName) = 0;

        virtual Entity& createEntity(const std::string& name = "") = 0;
        virtual Entity& createEntityWithUUID(const std::string& name, UUID id) = 0;
        virtual void destroyEntity(Entity& entity) = 0;
        virtual void onResize(uint32_t width, uint32_t height) = 0;

        virtual void onUpdateEditor(TimeStep deltaTime, const Ref<EditorCamera>& camera) = 0;
        virtual void onUpdateRuntime(TimeStep deltaTime) = 0;

    protected:
        virtual void onComponentAdded(Entity& e, Component& component) = 0;

        entt::registry entityRegistry;

        uint32_t viewportWidth = 0;
        uint32_t viewportHeight = 0;


        friend class Entity;
        friend class ProjectSerialiser;
    };
};  // namespace AnEngine

#endif
