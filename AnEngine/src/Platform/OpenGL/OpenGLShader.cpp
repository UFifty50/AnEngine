#include "aepch.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.hpp"

#include <glad/glad.h>

#include "File/InputFileStream.hpp"


namespace AnEngine {
    OpenGLShader::OpenGLShader(InputFileStream& vertShaderStream,
                               InputFileStream& fragShaderStream) {
        std::string vertShader;
        std::string fragShader;

        // Read the Vertex Shader code from the file
        AE_CORE_ASSERT(vertShaderStream.is_open(),
                       "Couldn't open " + vertShaderStream.getFilePath() + ".");
        vertShader = vertShaderStream.readAll();
        vertShaderStream.close();

        // Read the Fragment Shader code from the file
        AE_CORE_ASSERT(fragShaderStream.is_open(),
                       "Couldn't open " + fragShaderStream.getFilePath() + ".");
        fragShader = fragShaderStream.readAll();
        fragShaderStream.close();

        this->rendererID = this->compileAndCheckShaders(vertShader, fragShader);
    }

    OpenGLShader::OpenGLShader(const std::string& vertShaderSrc,
                               const std::string& fragShaderSrc) {
        this->rendererID =
            this->compileAndCheckShaders(vertShaderSrc, fragShaderSrc);
    }

    OpenGLShader::~OpenGLShader() { glDeleteProgram(this->rendererID); }

    uint32_t OpenGLShader::compileAndCheckShaders(
        const std::string& vertShaderSrc,
        const std::string& fragShaderSrc) const {
        // Create the shaders
        GLuint vertShaderID = glCreateShader(GL_VERTEX_SHADER);
        GLuint fragShaderID = glCreateShader(GL_FRAGMENT_SHADER);


        // Compile Vertex Shader
        AE_CORE_DEBUG("Compiling vertex shader : {0}", vertShaderID);
        char const* VertexSourcePointer = vertShaderSrc.c_str();
        glShaderSource(vertShaderID, 1, &VertexSourcePointer, NULL);
        glCompileShader(vertShaderID);


        // Check Vertex Shader
        GLint compileResult;
        glGetShaderiv(vertShaderID, GL_COMPILE_STATUS, &compileResult);
        if (compileResult == GL_FALSE) {
            GLint InfoLogLength = 0;
            glGetShaderiv(vertShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

            std::vector<GLchar> VertexShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(vertShaderID, InfoLogLength, &InfoLogLength,
                               &VertexShaderErrorMessage[0]);

            glDeleteShader(vertShaderID);

            AE_CORE_CRITICAL("{0}", VertexShaderErrorMessage.data());
            AE_CORE_ASSERT(false, "Vertex shader compilation failed.");
            return 0;
        }


        // Compile Fragment Shader
        AE_CORE_DEBUG("Compiling fragment shader : {0}", fragShaderID);
        char const* FragmentSourcePointer = fragShaderSrc.c_str();
        glShaderSource(fragShaderID, 1, &FragmentSourcePointer, NULL);
        glCompileShader(fragShaderID);


        // Check Fragment Shader
        glGetShaderiv(fragShaderID, GL_COMPILE_STATUS, &compileResult);
        if (compileResult == GL_FALSE) {
            GLint InfoLogLength = 0;
            glGetShaderiv(fragShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);

            std::vector<GLchar> FragmentShaderErrorMessage(InfoLogLength);
            glGetShaderInfoLog(fragShaderID, InfoLogLength, &InfoLogLength,
                               &FragmentShaderErrorMessage[0]);

            glDeleteShader(fragShaderID);
            glDeleteShader(vertShaderID);

            AE_CORE_CRITICAL("{0}", FragmentShaderErrorMessage.data());
            AE_CORE_ASSERT(false, "Fragment shader compilation failed.");
            return 0;
        }


        // Link the program
        AE_CORE_DEBUG("Linking program");
        GLuint programID = glCreateProgram();
        glAttachShader(programID, vertShaderID);
        glAttachShader(programID, fragShaderID);
        glLinkProgram(programID);


        // Check the program
        GLint linkResult = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, (int*)&linkResult);
        if (linkResult == GL_FALSE) {
            GLint InfoLogLength = 0;
            glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<GLchar> ProgramErrorMessage(InfoLogLength);
            glGetProgramInfoLog(programID, InfoLogLength, &InfoLogLength,
                                &ProgramErrorMessage.front());

            glDeleteProgram(programID);
            glDeleteShader(vertShaderID);
            glDeleteShader(fragShaderID);

            AE_CORE_CRITICAL("{0}", ProgramErrorMessage.data());
            AE_CORE_ASSERT(false, "Shader linking failed.");
            return programID;
        }


        glDetachShader(programID, vertShaderID);
        glDetachShader(programID, fragShaderID);

        glDeleteShader(vertShaderID);
        glDeleteShader(fragShaderID);

        return static_cast<uint32_t>(programID);
    }

    void OpenGLShader::bind() const { glUseProgram(this->rendererID); }

    void OpenGLShader::unbind() const { glUseProgram(0); }

    void OpenGLShader::uploadUniform(const std::string& name,
                                     std::any uniform) {
        GLint location = glGetUniformLocation(this->rendererID, name.c_str());
        if (location == -1) {
            AE_CORE_ERROR("Uniform {0} doesn't exist.", name);
            return;
        }

        if (uniform.type() == typeid(int32_t)) {
            glUniform1i(location, std::any_cast<int32_t>(uniform));
        } else if (uniform.type() == typeid(uint32_t)) {
            glUniform1ui(location, std::any_cast<uint32_t>(uniform));
        } else if (uniform.type() == typeid(bool)) {
            glUniform1i(location, std::any_cast<bool>(uniform));
        } else if (uniform.type() == typeid(float)) {
            glUniform1f(location, std::any_cast<float>(uniform));
        } else if (uniform.type() == typeid(double)) {
            glUniform1d(location, std::any_cast<double>(uniform));
        }


        else if (uniform.type() == typeid(glm::vec2)) {
            glm::vec2 vec = std::any_cast<glm::vec2>(uniform);
            glUniform2f(location, vec.x, vec.y);
        } else if (uniform.type() == typeid(glm::ivec2)) {
            glm::ivec2 vec = std::any_cast<glm::ivec2>(uniform);
            glUniform2i(location, vec.x, vec.y);
        } else if (uniform.type() == typeid(glm::uvec2)) {
            glm::uvec2 vec = std::any_cast<glm::uvec2>(uniform);
            glUniform2ui(location, vec.x, vec.y);
        } else if (uniform.type() == typeid(glm::dvec2)) {
            glm::dvec2 vec = std::any_cast<glm::dvec2>(uniform);
            glUniform2d(location, vec.x, vec.y);
        } else if (uniform.type() == typeid(glm::bvec2)) {
            glm::bvec2 vec = std::any_cast<glm::bvec2>(uniform);
            glUniform2i(location, vec.x, vec.y);
        }

        else if (uniform.type() == typeid(glm::vec3)) {
            glm::vec3 vec = std::any_cast<glm::vec3>(uniform);
            glUniform3f(location, vec.x, vec.y, vec.z);
        } else if (uniform.type() == typeid(glm::ivec3)) {
            glm::ivec3 vec = std::any_cast<glm::ivec3>(uniform);
            glUniform3i(location, vec.x, vec.y, vec.z);
        } else if (uniform.type() == typeid(glm::uvec3)) {
            glm::uvec3 vec = std::any_cast<glm::uvec3>(uniform);
            glUniform3ui(location, vec.x, vec.y, vec.z);
        } else if (uniform.type() == typeid(glm::dvec3)) {
            glm::dvec3 vec = std::any_cast<glm::dvec3>(uniform);
            glUniform3d(location, vec.x, vec.y, vec.z);
        } else if (uniform.type() == typeid(glm::bvec3)) {
            glm::bvec3 vec = std::any_cast<glm::bvec3>(uniform);
            glUniform3i(location, vec.x, vec.y, vec.z);
        }

        else if (uniform.type() == typeid(glm::vec4)) {
            glm::vec4 vec = std::any_cast<glm::vec4>(uniform);
            glUniform4f(location, vec.r, vec.b, vec.g, vec.a);
        } else if (uniform.type() == typeid(glm::ivec4)) {
            glm::ivec4 vec = std::any_cast<glm::ivec4>(uniform);
            glUniform4i(location, vec.r, vec.b, vec.g, vec.a);
        } else if (uniform.type() == typeid(glm::uvec4)) {
            glm::uvec4 vec = std::any_cast<glm::uvec4>(uniform);
            glUniform4ui(location, vec.r, vec.b, vec.g, vec.a);
        } else if (uniform.type() == typeid(glm::dvec4)) {
            glm::dvec4 vec = std::any_cast<glm::dvec4>(uniform);
            glUniform4d(location, vec.r, vec.b, vec.g, vec.a);
        } else if (uniform.type() == typeid(glm::bvec4)) {
            glm::bvec4 vec = std::any_cast<glm::bvec4>(uniform);
            glUniform4i(location, vec.r, vec.b, vec.g, vec.a);
        }


        else if (uniform.type() == typeid(glm::mat3)) {
            glUniformMatrix3fv(
                location, 1, GL_FALSE,
                glm::value_ptr(std::any_cast<glm::mat3>(uniform)));
        } else if (uniform.type() == typeid(glm::mat4)) {
            glUniformMatrix4fv(
                location, 1, GL_FALSE,
                glm::value_ptr(std::any_cast<glm::mat4>(uniform)));
        } else {
            AE_CORE_ASSERT(false, "Unknown uniform type.");
        }
    }
}  // namespace AnEngine
