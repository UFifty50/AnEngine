#ifndef OGLSHADERCOMPILE_HPP
#define OGLSHADERCOMPILE_HPP

#include <string>
#include "File/InputFileStream.hpp"


namespace AnEngine {
    class Shader {
    private:
        unsigned int rendererID = NULL;
        unsigned int compileAndCheckShaders(const std::string& vertShaderSrc, const std::string& fragShaderSrc);

    public:
        Shader(AnEngine::InputFileStream& vertShaderStream, AnEngine::InputFileStream& fragShaderStream);
        Shader(const std::string& vertShaderSrc, const std::string& fragShaderSrc);
        ~Shader();

        void bind() const;
        void unbind() const;
    };
}

#endif
