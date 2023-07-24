#ifndef SHADERUNIFORM_HPP
#define SHADERUNIFORM_HPP

#include <glm/glm.hpp>

#include <any>
#include <string>


namespace AnEngine {
    struct Sampler2D {
        uint32_t slot;
    };


    template <typename S>
    concept ScalarGLSLDataType = std::is_same_v<S, bool> || std::is_same_v<S, int32_t> ||
                                 std::is_same_v<S, uint32_t> ||
                                 std::is_same_v<S, float> || std::is_same_v<S, double>;

    template <typename V>
    concept VectorGLSLDataType =
        std::is_same_v<V, glm::bvec1> || std::is_same_v<V, glm::bvec2> ||
        std::is_same_v<V, glm::bvec3> || std::is_same_v<V, glm::bvec4> ||
        std::is_same_v<V, glm::ivec1> || std::is_same_v<V, glm::ivec2> ||
        std::is_same_v<V, glm::ivec3> || std::is_same_v<V, glm::ivec4> ||
        std::is_same_v<V, glm::uvec1> || std::is_same_v<V, glm::uvec2> ||
        std::is_same_v<V, glm::uvec3> || std::is_same_v<V, glm::uvec4> ||
        std::is_same_v<V, glm::vec1> || std::is_same_v<V, glm::vec2> ||
        std::is_same_v<V, glm::vec3> || std::is_same_v<V, glm::vec4> ||
        std::is_same_v<V, glm::dvec1> || std::is_same_v<V, glm::dvec2> ||
        std::is_same_v<V, glm::dvec3> || std::is_same_v<V, glm::dvec4>;

    template <typename M>
    concept MatrixGLSLDataType =
        std::is_same_v<M, glm::mat2> ||  // glm::mat2x3,  glm::mat2x4,
        std::is_same_v<M, glm::mat3> ||  // glm::mat3x2,  glm::mat3x4,
        std::is_same_v<M, glm::mat4>;    // glm::mat4x2,  glm::mat4x3,

    /* glm::dmat2, glm::dmat2x3, glm::dmat2x4,
       glm::dmat3, glm::dmat3x2, glm::dmat3x4,
       glm::dmat4, glm::dmat4x2, glm::dmat4x3>; */


    template <typename T>
    concept TextureGLSLDataType = std::is_same_v<T, Sampler2D>;

    template <typename T>
    concept GLSLDataType = ScalarGLSLDataType<T> || VectorGLSLDataType<T> ||
                           MatrixGLSLDataType<T> || TextureGLSLDataType<T>;


    struct ShaderUniformBase {
        std::string name;
        std::any uniform;
    };

    template <GLSLDataType UniformType>
    struct ShaderUniform : public ShaderUniformBase {
        UniformType uniform;

        ShaderUniform(const std::string& name, UniformType uniform)
            : ShaderUniformBase{name, uniform}, uniform{uniform} {}
    };

    class ShaderUniformVector : public std::vector<ShaderUniformBase> {
    public:
        using std::vector<ShaderUniformBase>::vector;

        std::any& operator[](const std::string& name) {
            for (auto& item : *this) {
                if (item.name == name) return item.uniform;
            }
            throw std::out_of_range("ShaderUniformVector::operator[] unable to find" +
                                    name);
        }

        std::any getOr(const std::string& name, std::any emptyType) const {
            for (auto& item : *this) {
                if (item.name == name) return item.uniform;
            }
            return emptyType;
        }
    };


};  // namespace AnEngine

#endif
