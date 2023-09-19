#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <glm/glm.hpp>

#include <functional>

#include "Core/Concepts.hpp"
#include "Scene/SceneCamera.hpp"
#include "Scene/ScriptableEntity.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
    struct TransformComponent {
        glm::mat4 Transform{1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::mat4& transform) : Transform(transform) {}

        operator glm::mat4&() { return Transform; }
        operator const glm::mat4&() const { return Transform; }
    };

    struct SpriteRendererComponent {
        glm::vec4 Colour{1.0f};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& colour) : Colour(colour) {}
    };

    struct TagComponent {
        std::string Tag;

        TagComponent() : Tag("Entity") {}
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}

        operator std::string&() { return Tag; }
    };

    struct CameraComponent {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;
    };

    struct NativeScriptComponent {
        ScriptableEntity* Instance;

        std::function<void()> instantiateScriptInstance;
        std::function<void()> deleteScriptInstance;

        std::function<void()> onCreate;
        std::function<void()> onDestroy;
        std::function<void(TimeStep)> onUpdate;

        template <Scriptable Script>
        void bind() {
            instantiateScriptInstance = [&]() { Instance = new Script(); };
            deleteScriptInstance = [&]() { delete (Script*)Instance; };
            onCreate = [&]() { ((Script*)Instance)->onCreate(); };
            onUpdate = [&](TimeStep dt) { ((Script*)Instance)->onUpdate(dt); };
        }
    };
}  // namespace AnEngine

#endif
