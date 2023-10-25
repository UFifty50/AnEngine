#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <functional>
#include <string>

#include "Scene/SceneCamera.hpp"
#include "Scene/ScriptableEntity.hpp"
#include "Time/TimeStep.hpp"


#define TRANSFORM_COMPONENT_ID 0
#define SPRITERENDERER_COMPONENT_ID 1
#define TAG_COMPONENT_ID 2
#define CAMERA_COMPONENT_ID 3
#define NATIVESCRIPT_COMPONENT_ID 4

namespace AnEngine {
    struct Component {
        virtual ~Component() = default;
        constexpr virtual uint32_t getID() = 0;
    };

    struct TransformComponent : Component {
        glm::vec3 Position{0.0f};
        glm::vec3 Rotation{0.0f};
        glm::vec3 Scale{1.0f};

        TransformComponent() = default;
        TransformComponent(const TransformComponent&) = default;
        TransformComponent(const glm::vec3& position) : Position(position) {}

        constexpr virtual uint32_t getID() override { return TRANSFORM_COMPONENT_ID; }

        operator const glm::mat4&() const {
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), Rotation.x, {1, 0, 0}) *
                                 glm::rotate(glm::mat4(1.0f), Rotation.y, {0, 1, 0}) *
                                 glm::rotate(glm::mat4(1.0f), Rotation.z, {0, 0, 1});

            return glm::translate(glm::mat4(1.0f), Position) * rotation *
                   glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent : Component {
        glm::vec4 Colour{1.0f};
        // TODO: Ref<Material>
        // Ref<Texture2D> Texture = {};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& colour) : Colour(colour) {}

        constexpr virtual uint32_t getID() override {
            return SPRITERENDERER_COMPONENT_ID;
        }
    };

    struct TagComponent : Component {
        std::string Tag;

        TagComponent() : Tag("Entity") {}
        TagComponent(const TagComponent&) = default;
        TagComponent(const std::string& tag) : Tag(tag) {}

        virtual uint32_t getID() override { return TAG_COMPONENT_ID; }

        constexpr operator std::string&() { return Tag; }
    };

    struct CameraComponent : Component {
        SceneCamera Camera;
        bool Primary = true;
        bool FixedAspectRatio = false;

        CameraComponent() = default;
        CameraComponent(const CameraComponent&) = default;

        constexpr virtual uint32_t getID() override { return CAMERA_COMPONENT_ID; }
    };


    struct NativeScriptComponent : Component {
        ScriptableEntity* Instance = nullptr;
        std::string Name;

        std::function<void()> instantiateScriptInstance = nullptr;
        std::function<void()> deleteScriptInstance = nullptr;

        NativeScriptComponent(std::string name) : Name(name) {}

        constexpr virtual uint32_t getID() override { return NATIVESCRIPT_COMPONENT_ID; }

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
