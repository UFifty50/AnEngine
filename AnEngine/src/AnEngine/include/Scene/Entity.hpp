#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <entt/entt.hpp>

// #include "Scene/Components.hpp"
#include "Core/Log.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine {

    class Entity {
    public:
        Entity() : entityHandle(entt::null), scene(nullptr) {}
        Entity(entt::entity handle, Scene* scene) : entityHandle(handle), scene(scene) {}
        Entity(const Entity& other) = default;

        template <typename T>
        bool hasComponent() {
            if (scene == nullptr || entityHandle == entt::null) return false;
            return scene->entityRegistry.any_of<T>(entityHandle);
        }

        template <typename T>
        T& getComponent() {
            AE_CORE_ASSERT(hasComponent<T>(), "Entity does not have component of type {0}!",
                           typeid(T).name());
            return scene->entityRegistry.get<T>(entityHandle);
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) {
            AE_CORE_ASSERT(!hasComponent<T>(), "Entity already has component of type {0}!",
                           typeid(T).name());

            T& component =
                scene->entityRegistry.emplace<T>(entityHandle, std::forward<Args>(args)...);
            scene->onComponentAdded(*this, component);
            return component;
        }

        template <typename... Ts>
        void removeComponent() {
            bool errorOccurred = false;

            (
                [&]() {
                    if (!hasComponent<Ts>()) {
                        AE_CORE_CRITICAL("Entity does not have components of type {0}!",
                                         typeid(Ts).name());
                        errorOccurred = true;
                    }
                }(),
                ...);

            if (errorOccurred) AE_CORE_ASSERT(false, "Failed to remove components!")

            scene->entityRegistry.remove<Ts...>(entityHandle);
        }

        /* template <class Script>
         void addNativeScript(std::string name) {
             AE_CORE_ASSERT(!hasComponent<Script>(),
                            "Entity already has script class {0}!",
                            typeid(Script).name());

             scene->entityRegistry.view<NativeScriptComponent>().each([&](auto e,
                                                                          auto& nsc) {
                 if (nsc.Name == name) {
                     AE_CORE_ASSERT(false, "Entity already has script name {0}!", name);
                 }
             });

             addComponent<NativeScriptComponent>(name).bind<Script>();
         }*/

        operator bool() const { return entityHandle != entt::null; }
        operator uint32_t() const { return (uint32_t)entityHandle; }
        operator entt::entity() const { return entityHandle; }

        bool operator==(const Entity& other) const {
            return entityHandle == other.entityHandle && scene == other.scene;
        }

    private:
        entt::entity entityHandle{entt::null};
        Scene* scene;
    };
}  // namespace AnEngine

#endif
