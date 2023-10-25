#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entt.hpp>

#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Entity;
    namespace Crank {
        class ScenesPanel;
    };

    class Scene {
    public:
        Scene() = default;
        Scene(std::string name) : name(name) {}
        ~Scene() = default;

        Entity createEntity(const std::string& name = "");

        void onUpdate(TimeStep deltaTime);
        void onResize(uint32_t width, uint32_t height);

        std::string getName() { return name; }

    private:
        std::string name = "Unnamed Scene";
        entt::registry entityRegistry;
        uint32_t viewportWidth = 0;
        uint32_t viewportHeight = 0;

        friend class Entity;
        friend class Crank::ScenesPanel;
    };
}  // namespace AnEngine

#endif
