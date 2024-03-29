#ifndef OGLSHADERCOMPILE_HPP
#define OGLSHADERCOMPILE_HPP

#include <glm/glm.hpp>

#include <glad/glad.h>

#include <any>
#include <string>

#include "Core/Core.hpp"
#include "File/InputFileStream.hpp"
#include "Renderer/Shader.hpp"


namespace AnEngine {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(InputFileStream& mixedShaderStream, const std::string& name = "");
        ~OpenGLShader();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void uploadUniform(const std::string& name, std::any uniform) override;

        virtual const std::string& getName() const override { return name; }

    private:
        RenderID rendererID = NULL;
        std::string name;
        std::string filePath;
        std::unordered_map<GLenum, std::vector<uint32_t>> vulkanSPRIV;
        std::unordered_map<GLenum, std::vector<uint32_t>> openglSPRIV;
        std::unordered_map<GLenum, std::string> openglSource;

        std::unordered_map<GLenum, std::string> preProcess(const std::string& source);

        void compileOrGetVulkanBinaries(
            const std::unordered_map<GLenum, std::string>& shaderSources);

        void compileOrGetOpenGLBinaries();

        GLuint createProgram();

        void reflect(GLenum shaderType, const std::vector<uint32_t>& spirvCode);
    };

    class ShaderParser {
    public:
        ShaderParser(const std::string& mixedShaderSrc);

        void parse();
        std::unordered_map<GLenum, std::string> getShaders() const;

    private:
        enum StrCode : uint8_t {
            VERTEX = 0,
            FRAGMENT = 1,
            GEOMETRY = 2,
            COMPUTE = 5,
            UNKNOWN = 255
        };

        std::string mixedShaderSrc;
        GLenum shaderType;
        std::unordered_map<GLenum, std::string> shaders;

        StrCode hashedType(const std::string& type) {
            if (type == "vertex") return VERTEX;
            if (type == "fragment" || type == "pixel") return FRAGMENT;
            if (type == "geometry") return GEOMETRY;
            if (type == "compute") return COMPUTE;
            return UNKNOWN;
        }
    };
}  // namespace AnEngine

#endif
