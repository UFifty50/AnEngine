#ifndef COMPONENTS_HPP
#define COMPONENTS_HPP

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>

#include <cstdint>
#include <functional>
#include <string>

#include "Core/ForwardDecls.hpp"

#include "Core/UUID.hpp"
#include "Project/Resources/Material.hpp"
#include "Renderer/Camera/SceneCamera.hpp"


namespace AnEngine {
struct Component {
    enum class Type : uint8_t {
        ID = 0,
        Tag,
        Camera,
        Transform,
        ObjectRenderer,
        SpriteRenderer,
        NativeScript,
    };

    virtual ~Component() = default;
    constexpr virtual Type getID() = 0;
    constexpr virtual std::string getName() = 0;
};

struct IDComponent : Component {
    UUID uuid;

    IDComponent() = default;
    IDComponent(UUID id) : uuid(id) {}
    IDComponent(const IDComponent&) = default;

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::ID; }
    constexpr static std::string GetName() { return "IDComponent"; }
};

struct TransformComponent : Component {
    glm::vec3 Position{0.0f};
    glm::vec3 Rotation{0.0f};
    glm::vec3 Scale{1.0f};

    TransformComponent() = default;
    TransformComponent(const TransformComponent&) = default;
    TransformComponent(const glm::vec3& position) : Position(position) {}

    operator glm::mat4() const {
        return translate(glm::mat4(1.0f), Position) *
            toMat4(glm::quat(Rotation)) * scale(glm::mat4(1.0f), Scale);
    }

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::Transform; }
    constexpr static std::string GetName() { return "TransformComponent"; }
};

struct ObjectRendererComponent : Component {
    Material ObjMaterial;

    ObjectRendererComponent() = default;
    ObjectRendererComponent(const ObjectRendererComponent&) = default;
    ObjectRendererComponent(const Material& material) : ObjMaterial(material) {}

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::ObjectRenderer; }
    constexpr static std::string GetName() { return "ObjectRendererComponent"; }
};

struct SpriteRendererComponent : Component {
    Material Mat;

    SpriteRendererComponent() = default;
    SpriteRendererComponent(const SpriteRendererComponent&) = default;
    SpriteRendererComponent(const Material& material) : Mat(material) {}

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::SpriteRenderer; }
    constexpr static std::string GetName() { return "SpriteRendererComponent"; }
};

struct TagComponent : Component {
    std::string Tag;

    TagComponent() : Tag("Entity") {}
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) : Tag(tag) {}

    constexpr operator std::string&() { return Tag; }

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::Tag; }
    constexpr static std::string GetName() { return "TagComponent"; }
};

struct CameraComponent : Component {
    SceneCamera Camera;
    bool Primary = true;
    bool FixedAspectRatio = false;

    CameraComponent() = default;
    CameraComponent(const CameraComponent&) = default;

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::Camera; }
    constexpr static std::string GetName() { return "CameraComponent"; }
};

struct NativeScriptComponent : Component {
    ScriptableEntity* Instance = nullptr;
    std::string Name;

    std::function<void()> instantiateScriptInstance = nullptr;
    std::function<void()> deleteScriptInstance = nullptr;

    NativeScriptComponent(std::string name) : Name(name) {}

    template <class Script>
    void bind() {
        instantiateScriptInstance = [&]() { Instance = new Script(); };
        deleteScriptInstance = [&]() {
            delete static_cast<Script*>(Instance);
            Instance = nullptr;
        };
    }

    constexpr Type getID() override { return GetID(); }
    constexpr std::string getName() override { return GetName(); }

    constexpr static Type GetID() { return Type::NativeScript; }
    constexpr static std::string GetName() { return "NativeScriptComponent"; }
};
} // namespace AnEngine

#endif
