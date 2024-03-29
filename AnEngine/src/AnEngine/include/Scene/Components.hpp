#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <string>

#include "Renderer/Camera/SceneCamera.hpp"
#include "Scene/ScriptableEntity.hpp"
#include "Time/TimeStep.hpp"


#define TRANSFORM_COMPONENT_ID 0
#define SPRITERENDERER_COMPONENT_ID 1
#define MATERIAL_COMPONENT_ID 2
#define TAG_COMPONENT_ID 3
#define CAMERA_COMPONENT_ID 4
#define NATIVESCRIPT_COMPONENT_ID 5

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

        operator glm::mat4() const {
            return glm::translate(glm::mat4(1.0f), Position) *
                   glm::toMat4(glm::quat(Rotation)) * glm::scale(glm::mat4(1.0f), Scale);
        }
    };

    struct SpriteRendererComponent : Component {
        glm::vec4 Colour{1.0f};
        // TODO: Ref<Material>
        // Ref<Texture2D> Texture = {};

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const glm::vec4& colour) : Colour(colour) {}

        constexpr virtual uint32_t getID() override { return SPRITERENDERER_COMPONENT_ID; }
    };

    struct MaterialComponent : Component {
        MaterialComponent() = default;
        MaterialComponent(const MaterialComponent&) = default;

        constexpr virtual uint32_t getID() override { return MATERIAL_COMPONENT_ID; }
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
