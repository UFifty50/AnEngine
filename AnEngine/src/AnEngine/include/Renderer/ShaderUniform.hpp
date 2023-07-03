#ifndef SHADERUNIFORM_HPP
#define SHADERUNIFORM_HPP

#include <any>
#include <string>
#include <glm/glm.hpp>


namespace AnEngine {
    template <typename S>
    concept ScalarGLSLDataType = std::_Is_any_of_v<S, bool, int32_t, uint32_t, float, double>;

    template <typename V>
    concept VectorGLSLDataType =
        std::_Is_any_of_v<V, glm::bvec1, glm::bvec2, glm::bvec3, glm::bvec4,
        glm::ivec1, glm::ivec2, glm::ivec3, glm::ivec4,
        glm::uvec1, glm::uvec2, glm::uvec3, glm::uvec4,
        glm::vec1, glm::vec2, glm::vec3, glm::vec4,
        glm::dvec1, glm::dvec2, glm::dvec3, glm::dvec4>;

    template <typename M>
    concept MatrixGLSLDataType =
        std::_Is_any_of_v<M, glm::mat2, // glm::mat2x3,  glm::mat2x4,
        glm::mat3, // glm::mat3x2,  glm::mat3x4,
        glm::mat4 // glm::mat4x2,  glm::mat4x3,
        >;

    /* glm::dmat2, glm::dmat2x3, glm::dmat2x4,
       glm::dmat3, glm::dmat3x2, glm::dmat3x4,
       glm::dmat4, glm::dmat4x2, glm::dmat4x3>; */

    template <typename T>
    concept GLSLDataType = ScalarGLSLDataType<T> || VectorGLSLDataType<T> || MatrixGLSLDataType<T>;

    struct ShaderUniformBase {
        std::string name;
        std::any uniform;
    };

    template <GLSLDataType UniformType>
    struct ShaderUniform : public ShaderUniformBase {
        UniformType uniform;

        ShaderUniform(const std::string& name, UniformType uniform)
            : ShaderUniformBase{ name, uniform }, uniform{ uniform } {}
    };
};

#endif
