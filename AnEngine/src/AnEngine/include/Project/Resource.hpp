#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <filesystem>
#include <string>

#include "Core/Core.hpp"
#include "Core/Log.hpp"
#include "Core/UUID.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
class Scene;

struct Resource {
    enum class Type : uint8_t { Other, Material, Model, Texture, Shader, Scene2D, Scene3D };

    Type type;
    UUID uuid;
    std::string name;


    Ref<Scene> asScene() {
        if (type == Type::Scene2D || type == Type::Scene3D) {
            return std::reinterpret_pointer_cast<Scene>(MakeRef<Resource>(*this));
        }

        AE_CORE_ERROR("Resource \"{}\" (UUID {}) is not a scene!", name, uuid.toString());
        return nullptr;
    };

    std::string typeToString() const { return TypeToString(type); }

    static std::string TypeToString(const Type type) {
        switch (type) {
            case Type::Material: return "Material";
            case Type::Model: return "Model";
            case Type::Texture: return "Texture";
            case Type::Shader: return "Shader";
            case Type::Scene2D: return "Scene2D";
            case Type::Scene3D: return "Scene3D";
            default: return "Other";
        }
    }
};
}; // namespace AnEngine

#endif
