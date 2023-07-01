#ifndef OGLSHADERCOMPILE_HPP
#define OGLSHADERCOMPILE_HPP

#include <string>

#include "Renderer/Shader.hpp"
#include "File/InputFileStream.hpp"


namespace AnEngine {
    class OpenGLShader : public Shader {
    private:
        uint32_t rendererID = NULL;
        uint32_t compileAndCheckShaders(const std::string& vertShaderSrc, const std::string& fragShaderSrc) const override;

    public:
        OpenGLShader(AnEngine::InputFileStream& vertShaderStream, AnEngine::InputFileStream& fragShaderStream);
        OpenGLShader(const std::string& vertShaderSrc, const std::string& fragShaderSrc);
        ~OpenGLShader();

        virtual void bind() const;
        virtual void unbind() const;
    };
}

#endif
