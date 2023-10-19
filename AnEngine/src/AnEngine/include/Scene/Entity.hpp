#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <entt/entt.hpp>

// #include "Scene/Components.hpp"
#include "Core/Log.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine {

    class Entity {
    public:
        Entity() : entityHandle(entt::null), scene(new Scene()) {}
        Entity(entt::entity handle, Scene* scene) : entityHandle(handle), scene(scene) {}
        Entity(const Entity& other) = default;

        template <typename T>
        bool hasComponent() {
            return scene->entityRegistry.any_of<T>(entityHandle);
        }

        template <typename T>
        T& getComponent() {
            AE_CORE_ASSERT(hasComponent<T>(),
                           "Entity does not have component of type {0}!",
                           typeid(T).name());
            return scene->entityRegistry.get<T>(entityHandle);
        }

        template <typename T, typename... Args>
        T& addComponent(Args&&... args) {
            AE_CORE_ASSERT(!hasComponent<T>(),
                           "Entity already has component of type {0}!",
                           typeid(T).raw_name());
            return scene->entityRegistry.emplace<T>(entityHandle,
                                                    std::forward<Args>(args)...);
        }

        template <typename... Ts>
        void removeComponent() {
            bool errorOccurred = false;

            (
                [&]() {
                    if (!hasComponent<Ts>()) {
                        AE_CORE_CRITICAL("Entity does not have components of type {0}!",
                                         typeid(Ts).raw_name());
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
                            typeid(Script).raw_name());

             scene->entityRegistry.view<NativeScriptComponent>().each([&](auto e,
                                                                          auto& nsc) {
                 if (nsc.Name == name) {
                     AE_CORE_ASSERT(false, "Entity already has script name {0}!", name);
                 }
             });

             addComponent<NativeScriptComponent>(name).bind<Script>();
         }*/

        operator bool() const { return entityHandle != entt::null; }

    private:
        entt::entity entityHandle{entt::null};
        Scene* scene;
    };
}  // namespace AnEngine

#endif
