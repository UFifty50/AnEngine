#include "aepch.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "OpenGLShader.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <cctype>

#include "File/InputFileStream.hpp"
#include "Renderer/ShaderUniform.hpp"


namespace AnEngine {
    OpenGLShader::OpenGLShader(InputFileStream& mixedShaderStream,
                               const std::string& name) {
        if (!mixedShaderStream.is_open()) {
            AE_CORE_CRITICAL("Cannot compile as {0} is not open.",
                             mixedShaderStream.getFilePath());
            return;
        }


        std::unordered_map<GLenum, std::string> shaderSources =
            this->preProcess(mixedShaderStream.readAll());
        mixedShaderStream.close();

        name != "" ? this->name = name : this->name = mixedShaderStream.getFileName();

        this->rendererID = this->compile(shaderSources);
    }

    std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(
        const std::string& mixedShaderSrc) {
        std::unordered_map<GLenum, std::string> shaderSources;

        ShaderParser parser(mixedShaderSrc);
        parser.parse();
        for (auto& [shaderType, shaderSrc] : parser.getShaders()) {
            shaderSources[shaderType] = shaderSrc;
        }

        return shaderSources;
    }

    uint32_t OpenGLShader::compile(
        const std::unordered_map<uint32_t, std::string>& shaderSources) const {
        std::vector<GLuint> shaderIDs;
        shaderIDs.resize(shaderSources.size());

        GLuint program = glCreateProgram();

        AE_CORE_DEBUG("Compiling shader {0}", program);

        for (auto& [shaderType, shaderSrc] : shaderSources) {
            GLuint shader = glCreateShader(shaderType);

            const GLchar* source = shaderSrc.c_str();
            glShaderSource(shader, 1, &source, 0);
            glCompileShader(shader);

            GLint compileResult;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
            if (compileResult == GL_FALSE) {
                GLint InfoLogLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &InfoLogLength);
                std::vector<GLchar> infoLog(InfoLogLength);
                glGetShaderInfoLog(shader, InfoLogLength, &InfoLogLength, &infoLog[0]);
                AE_CORE_CRITICAL("Shader compilation failed: {0}", infoLog.data());
                glDeleteShader(shader);
                return -1;
            }

            glAttachShader(program, shader);
            shaderIDs.push_back(shader);
        }

        glLinkProgram(program);

        GLint linkResult = 0;
        glGetProgramiv(program, GL_LINK_STATUS, (int*)&linkResult);
        if (linkResult == GL_FALSE) {
            GLint InfoLogLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &InfoLogLength);
            std::vector<GLchar> infoLog(InfoLogLength);
            glGetProgramInfoLog(program, InfoLogLength, &InfoLogLength,
                                &infoLog.front());  // .front called on empty vector

            glDeleteProgram(program);
            for (auto& shader : shaderIDs) {
                glDeleteShader(shader);
            }

            AE_CORE_CRITICAL("Shader linking failed: {0}", infoLog.data());
            return -1;
        }


        for (auto& shader : shaderIDs) {
            glDetachShader(program, shader);
            glDeleteShader(shader);
        }

        AE_CORE_DEBUG("Shader {0} compiled", program);

        return program;
    }

    OpenGLShader::~OpenGLShader() { glDeleteProgram(this->rendererID); }

    void OpenGLShader::bind() const { glUseProgram(this->rendererID); }

    void OpenGLShader::unbind() const { glUseProgram(0); }

    void OpenGLShader::uploadUniform(const std::string& name, std::any uniform) {
        GLint location = glGetUniformLocation(this->rendererID, name.c_str());
        if (location == -1) {
            AE_CORE_ERROR("Uniform {0} doesn't exist.", name);
            return;
        }
        // integral types
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

        // 2D vectors
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

        // 3D vectors
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

        // 4D vectors
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


        // Matrices
        else if (uniform.type() == typeid(glm::mat3)) {
            glUniformMatrix3fv(location, 1, GL_FALSE,
                               glm::value_ptr(std::any_cast<glm::mat3>(uniform)));
        } else if (uniform.type() == typeid(glm::mat4)) {
            glUniformMatrix4fv(location, 1, GL_FALSE,
                               glm::value_ptr(std::any_cast<glm::mat4>(uniform)));
        }

        // Textures
        else if (uniform.type() == typeid(Sampler2D)) {
            uint32_t slot = std::any_cast<Sampler2D>(uniform).slot;
            glUniform1i(location, slot);
        }

        else {
            AE_CORE_ASSERT(false, "Unknown uniform type.");
        }
    }


    ShaderParser::ShaderParser(const std::string& mixedShaderSrc)
        : mixedShaderSrc(mixedShaderSrc) {}

    void ShaderParser::parse() {
        std::string line;
        std::stringstream ss(mixedShaderSrc);

        std::string shaderTokenA = "#shader";
        std::string shaderTokenB = "#type";

        while (std::getline(ss, line)) {
            if (line.starts_with(shaderTokenA) || line.starts_with(shaderTokenB)) {
                std::string shaderTypeStr = line.find_first_of(' ') != std::string::npos
                                                ? line.substr(line.find_first_of(' '))
                                                : "";

                auto trim = std::ranges::remove_if(shaderTypeStr, [](unsigned char c) {
                                return std::isspace(c);
                            }).begin();
                shaderTypeStr.erase(trim, shaderTypeStr.end());

                if (shaderTypeStr.empty()) {
                    AE_CORE_ASSERT(false, "Shader type must be specified.");
                }

                switch (hashedType(shaderTypeStr)) {
                    case VERTEX:
                        shaderType = GL_VERTEX_SHADER;
                        break;
                    case FRAGMENT:
                        shaderType = GL_FRAGMENT_SHADER;
                        break;
                    case GEOMETRY:
                        shaderType = GL_GEOMETRY_SHADER;
                        break;
                    case COMPUTE:
                        shaderType = GL_COMPUTE_SHADER;
                        break;
                    default:
                        AE_CORE_ASSERT(false, "Unknown shader type: \"{0}\"",
                                       shaderTypeStr);
                        break;
                }

                if (shaders.find(shaderType) != shaders.end()) {
                    AE_CORE_ASSERT(false, "{0} Shader already defined.", shaderTypeStr);
                }
            } else {
                shaders[shaderType] += line + "\n";
            }
        }
    }

    std::unordered_map<GLenum, std::string> ShaderParser::getShaders() const {
        return shaders;
    }
}  // namespace AnEngine
