#ifndef AE_MATHS_HPP
#define AE_MATHS_HPP

#include <glm/glm.hpp>

#include <array>
#include <optional>


namespace AnEngine::Maths {
    // returns optional of [translation, rotation, scale]
    std::optional<std::array<glm::vec3, 3>> decomposeTranslation(
        const glm::mat4& transform);
};  // namespace AnEngine::Maths

#endif
