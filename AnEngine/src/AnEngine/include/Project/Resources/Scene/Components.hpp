#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <functional>
#include <string>

#include "Core/UUID.hpp"
#include "Project/Resources/Material.hpp"
#include "Project/Resources/Scene/ScriptableEntity.hpp"
#include "Renderer/Camera/SceneCamera.hpp"
#include "Texture/Texture2D.hpp"
#include "Time/TimeStep.hpp"


#define ID_COMPONENT_ID 0
#define TAG_COMPONENT_ID 1
#define CAMERA_COMPONENT_ID 2
#define TRANSFORM_COMPONENT_ID 3
#define OBJECTRENDERER_COMPONENT_ID 4
#define SPRITERENDERER_COMPONENT_ID 5
#define NATIVESCRIPT_COMPONENT_ID 6

namespace AnEngine {
    struct Component {
        virtual ~Component() = default;
        constexpr virtual uint32_t getID() = 0;
    };


    struct IDComponent : Component {
        UUID uuid;

        IDComponent() = default;
        IDComponent(UUID id) : uuid(id) {}
        IDComponent(const IDComponent&) = default;

        constexpr uint32_t getID() override { return ID_COMPONENT_ID; }
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

    struct ObjectRendererComponent : Component {
        Material ObjMaterial;

        ObjectRendererComponent() = default;
        ObjectRendererComponent(const ObjectRendererComponent&) = default;
        ObjectRendererComponent(const Material& material) : ObjMaterial(material) {}

        constexpr virtual uint32_t getID() override { return OBJECTRENDERER_COMPONENT_ID; }
    };

    struct SpriteRendererComponent : Component {
        Material Mat;

        SpriteRendererComponent() = default;
        SpriteRendererComponent(const SpriteRendererComponent&) = default;
        SpriteRendererComponent(const Material& material) : Mat(material) {}

        constexpr virtual uint32_t getID() override { return SPRITERENDERER_COMPONENT_ID; }
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
