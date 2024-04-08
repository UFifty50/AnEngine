#ifndef RESOURCE_HPP
#define RESOURCE_HPP

#include <string>

#include "Core/UUID.hpp"


namespace AnEngine {
    struct Resource {
        enum class Type { Other, Material, Model, Texture, Shader, Scene2D, Scene3D };

        Type type;
        AnEngine::UUID uuid;
        std::string name;
    };
};  // namespace AnEngine

#endif
