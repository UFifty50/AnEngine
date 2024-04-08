#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

#include <optional>

#include "Core/Core.hpp"
#include "Core/UUID.hpp"
#include "Project/Resource.hpp"
#include "Renderer/Shader.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    struct Material : Resource {
        glm::vec4 colour{1.0f};
        Ref<Texture2D> texture{nullptr};
        // TODO: Ref<Texture2D> Albedo{nullptr};
        // TODO: Ref<Texture2D> Normal{nullptr};
        // TODO: Ref<Texture2D> Specular{nullptr};
        // TODO: Ref<Texture2D> Emissive{nullptr};
        // TODO: Ref<Shader> shader{nullptr};


        Material(std::string name = "Unnamed Material")
            : Resource{Resource::Type::Material, {}, name} {}
        Material(std::string name, AnEngine::UUID uuid)
            : Resource{Resource::Type::Material, uuid, name} {}
        Material(nullptr_t) : Resource{Resource::Type::Material, nullptr, ""} {}
        ~Material() = default;

        std::optional<Ref<Texture2D>> getTexture() const {
            if (texture)
                return texture;
            else
                return std::nullopt;
        }

        bool isNull() { return uuid.isNull(); }
        bool operator==(const Material& other) const { return uuid == other.uuid; };
    };
};  // namespace AnEngine

#endif
