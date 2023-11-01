#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entt.hpp>

#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Entity;
    struct Component;
    namespace Crank {
        class ScenesPanel;
    };

    class Scene {
    public:
        Scene() = default;
        Scene(std::string name) : name(name) {}
        ~Scene() = default;

        void clear() {
            name = "";
            entityRegistry.clear();
        }

        Entity createEntity(const std::string& name = "");
        void destroyEntity(Entity entity);

        void onUpdate(TimeStep deltaTime);
        void onResize(uint32_t width, uint32_t height);

        Entity getPrimaryCamera();

        std::string getName() { return name; }

    private:
        void onComponentAdded(Entity e, Component& component);

        std::string name = "Unnamed Scene";
        entt::registry entityRegistry;
        uint32_t viewportWidth = 0;
        uint32_t viewportHeight = 0;

        friend class Entity;
        friend class SceneSerialiser;
        friend class Crank::ScenesPanel;
    };
}  // namespace AnEngine

#endif
