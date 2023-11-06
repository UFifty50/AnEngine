#include "aepch.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/matrix_decompose.hpp>

#include <array>
#include <optional>

#include "Maths/Maths.hpp"


namespace AnEngine {
    // returns optional of [translation, rotation, scale]
    std::optional<std::array<glm::vec3, 3>> Maths::decomposeTranslation(
        const glm::mat4& transform) {
        // From glm::decompose in matrix_decompose.inl

        glm::mat4 localMatrix(transform);

        if (glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>())) {
            return std::nullopt;
        }

        if (glm::epsilonNotEqual(localMatrix[0][3], 0.0f, glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[1][3], 0.0f, glm::epsilon<float>()) ||
            glm::epsilonNotEqual(localMatrix[2][3], 0.0f, glm::epsilon<float>())) {
            localMatrix[0][3] = localMatrix[1][3] = localMatrix[2][3] = 0.0f;
            localMatrix[3][3] = 1.0f;
        }

        glm::vec3 translation = glm::vec3(localMatrix[3]);
        glm::vec3 rotation = glm::vec3(0.0f);
        glm::vec3 scale = glm::vec3(1.0f);

        localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

        glm::vec3 row[3];

        for (glm::length_t i = 0; i < 3; ++i)
            for (glm::length_t j = 0; j < 3; ++j) row[i][j] = localMatrix[i][j];

        scale.x = glm::length(row[0]);
        row[0] = glm::detail::scale(row[0], 1.0f);
        scale.y = glm::length(row[1]);
        row[1] = glm::detail::scale(row[1], 1.0f);
        scale.z = glm::length(row[2]);
        row[2] = glm::detail::scale(row[2], 1.0f);

        rotation.y = glm::asin(-row[0][2]);
        if (glm::cos(rotation.y) != 0) {
            rotation.x = atan2(row[1][2], row[2][2]);
            rotation.z = atan2(row[0][1], row[0][0]);
        } else {
            rotation.x = atan2(-row[2][0], row[1][1]);
            rotation.z = 0;
        }

        std::array<glm::vec3, 3> decomposedTransform;
        decomposedTransform[0] = translation;
        decomposedTransform[1] = rotation;
        decomposedTransform[2] = scale;

        return decomposedTransform;
    }
}  // namespace AnEngine
