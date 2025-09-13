#ifndef ENTITY_HPP
#define ENTITY_HPP

#define NOMINMAX
#include <cstdint>
#include <type_traits>
#include <entt/entt.hpp>

#include "Core/ForwardDecls.hpp"

#include "Scene.hpp"
#include "Core/Concepts.hpp"
#include "Core/Core.hpp"
#include "Core/Log.hpp"


namespace AnEngine {
class Entity {
public:
    Entity() : scene(nullptr) {}
    Entity(entt::entity handle, Scene* scene) : entityHandle(handle), scene(scene) {}
    Entity(const Entity& other) = default;

    template <IsComponent C>
    bool hasComponent() const {
        if (scene == nullptr || entityHandle == entt::null) return false;
        return scene->entityRegistry->any_of<C>(entityHandle);
    }

    template <IsComponent C>
    C& getComponent() {
        std::string name = C::GetName();
        AE_CORE_ASSERT(hasComponent<C>(), "Entity does not have component of type {0}!", name)
        return scene->entityRegistry->get<C>(entityHandle);
    }

    template <IsComponent C, typename... Args>
    C& addComponent(Args&&... args) {
        std::string name = C::GetName();
        AE_CORE_ASSERT(!hasComponent<C>(), "Entity already has component of type {0}!", name)

        C& component =
            scene->entityRegistry->emplace<C>(entityHandle, std::forward<Args>(args)...);
        scene->onComponentAdded(*this, component);
        return component;
    }

    template <IsComponent... Cs>
    void removeComponent() {
        bool errorOccurred = false;

        (
            [&]() {
                if (!hasComponent<Cs>()) {
                    std::string name = Cs::GetName();
                    AE_CORE_CRITICAL("Entity does not have components of type {0}!", name);
                    errorOccurred = true;
                }
            }(),
            ...);

        if (errorOccurred) AE_CORE_ASSERT(false, "Failed to remove components!")

        scene->entityRegistry->remove<Cs...>(entityHandle);
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
    operator uint32_t() const { return static_cast<uint32_t>(entityHandle); }
    operator entt::entity() const { return entityHandle; }

    bool operator==(const Entity& other) const {
        return entityHandle == other.entityHandle && scene == other.scene;
    }

private:
    entt::entity entityHandle{entt::null};
    Scene* scene;
};
} // namespace AnEngine

#endif
