#ifndef SCENE_HPP
#define SCENE_HPP

#include <entt/entt.hpp>

#include "Time/TimeStep.hpp"


namespace AnEngine {
    class Entity;

    class Scene {
    public:
        Scene() = default;
        ~Scene() = default;

        Entity createEntity(const std::string& name = "");

        void onUpdate(TimeStep deltaTime);
        void onResize(uint32_t width, uint32_t height);

    private:
        entt::registry entityRegistry;
        uint32_t viewportWidth = 0;
        uint32_t viewportHeight = 0;

        friend class Entity;
    };
}  // namespace AnEngine

#endif
