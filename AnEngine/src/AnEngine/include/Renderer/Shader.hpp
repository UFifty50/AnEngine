#ifndef SHADER_HPP
#define SHADER_HPP

#include <vector>

#include "File/InputFileStream.hpp"


namespace AnEngine {
    class Shader {
    public:
        virtual ~Shader() = default;

        static Shader* create(InputFileStream& vertShaderStream, InputFileStream& fragShaderStream);
        static Shader* create(const std::string& vertShaderSrc, const std::string& fragShaderSrc);

        virtual void bind() const = 0;
        virtual void unbind() const = 0;
    
    protected:
        virtual uint32_t compileAndCheckShaders(const std::string& vertShaderSrc, const std::string& fragShaderSrc) const = 0;
    };
}

#endif
