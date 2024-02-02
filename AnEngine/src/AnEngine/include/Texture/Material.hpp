#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

#include <optional>

#include "Core/Core.hpp"
#include "Renderer/Shader.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    struct Material {
        // TODO: uuid_t uuid;
        glm::vec4 colour{1.0f};
        Ref<Texture2D> texture{nullptr};
        std::string temporary;
        // TODO: Ref<Texture2D> Albedo{nullptr};
        // TODO: Ref<Texture2D> Normal{nullptr};
        // TODO: Ref<Texture2D> Specular{nullptr};
        // TODO: Ref<Texture2D> Emissive{nullptr};
        // TODO: Ref<Shader> shader{nullptr};


        Material() = default;
        Material(const std::string& temporary) : temporary(temporary) {}
        ~Material() = default;

        std::optional<Ref<Texture2D>> getTexture() {
            if (texture)
                return texture;
            else
                return std::nullopt;
        }

        // TODO: use UUID
        bool operator==(const Material& other) const {
            if (temporary == "Empty" || other.temporary == "Empty") return false;

            if ((texture && !other.texture) || (!texture && other.texture))
                return false;
            else if (texture && other.texture)
                return texture == other.texture;
            else
                return colour == other.colour;
        }
    };
};  // namespace AnEngine

// From GLM
void hash_combine(size_t& seed, size_t hash);

template <>
struct std::hash<AnEngine::Material> {
    size_t operator()(const AnEngine::Material& m) const {
        size_t h1 = std::hash<float>{}(m.colour.r);
        size_t h2 = std::hash<float>{}(m.colour.g);
        size_t h3 = std::hash<float>{}(m.colour.b);
        size_t h4 = std::hash<float>{}(m.colour.a);
        size_t h5 = m.texture ? std::hash<int>{}(m.texture->getSampler()) : 0;

        size_t hashSum = 0;
        hash_combine(hashSum, h1);
        hash_combine(hashSum, h2);
        hash_combine(hashSum, h3);
        hash_combine(hashSum, h4);
        hash_combine(hashSum, h5);

        return hashSum;
    }
};

template <>
struct std::less<AnEngine::Material> {
    size_t operator()(const AnEngine::Material& m1, const AnEngine::Material& m2) const {
        return std::hash<AnEngine::Material>{}(m1) < std::hash<AnEngine::Material>{}(m2);
    }
};

#endif
