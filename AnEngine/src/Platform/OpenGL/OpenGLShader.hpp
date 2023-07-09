#ifndef OGLSHADERCOMPILE_HPP
#define OGLSHADERCOMPILE_HPP

#include <glm/glm.hpp>

#include <any>
#include <string>

#include "File/InputFileStream.hpp"
#include "Renderer/Shader.hpp"


namespace AnEngine {
    class OpenGLShader : public Shader {
    public:
        OpenGLShader(InputFileStream& vertShaderStream,
                     InputFileStream& fragShaderStream);

        OpenGLShader(const std::string& vertShaderSrc,
                     const std::string& fragShaderSrc);
        ~OpenGLShader();

        virtual void bind() const override;
        virtual void unbind() const override;

        virtual void uploadUniform(const std::string& name,
                                   std::any uniform) override;

    private:
        uint32_t rendererID = NULL;

        uint32_t compileAndCheckShaders(
            const std::string& vertShaderSrc,
            const std::string& fragShaderSrc) const override;
    };
}  // namespace AnEngine

#endif
