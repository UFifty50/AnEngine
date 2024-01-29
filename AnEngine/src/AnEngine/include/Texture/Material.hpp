#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <glm/glm.hpp>

#include <optional>

#include "Core/Core.hpp"
#include "Texture/Texture2D.hpp"


namespace AnEngine {
    /// Material class
    /// Contains all the information needed to render detail to an object/sprite
    ///
    /// TODO:
    /// - Shader
    /// - Tint
    /// - Albedo Map
    /// - Normal Map
    /// - Specular Map
    /// - Emissive Map

    struct Material {
        glm::vec4 colour{1.0f};
        Ref<Texture2D> texture{nullptr};


        Material() = default;
        ~Material() = default;

        std::optional<Ref<Texture2D>> getTexture() {
            if (texture)
                return texture;
            else
                return std::nullopt;
        }
    };

};  // namespace AnEngine

#endif
