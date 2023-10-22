#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <glm/glm.hpp>

#include <functional>
#include <string>

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
        // TODO: Ref<Material>
        // Ref<Texture2D> Texture = {};

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
        ScriptableEntity* Instance = nullptr;
        std::string Name;

        std::function<void()> instantiateScriptInstance = nullptr;
        std::function<void()> deleteScriptInstance = nullptr;

        NativeScriptComponent(std::string name) : Name(name) {}

        template <class Script>
        void bind() {
            instantiateScriptInstance = [&]() { Instance = new Script(); };
            deleteScriptInstance = [&]() {
                delete (Script*)Instance;
                Instance = nullptr;
            };
        }
    };
}  // namespace AnEngine

#endif
