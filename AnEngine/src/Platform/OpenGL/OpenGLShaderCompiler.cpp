#include "aepch.hpp"

#include "OpenGLShaderCompiler.hpp"
#include "File/InputFileStream.hpp"

#include <glad/glad.h>


namespace AnEngine {
    Shader::Shader(AnEngine::InputFileStream& vertShaderStream, AnEngine::InputFileStream& fragShaderStream) {
        std::string vertShader;
        std::string fragShader;
        
        // Read the Vertex Shader code from the file
        AE_CORE_ASSERT(vertShaderStream.is_open(), "Couldn't open " + vertShaderStream.getFilePath() + ".");
        vertShader = vertShaderStream.readAll();
        vertShaderStream.close();

        // Read the Fragment Shader code from the file
        AE_CORE_ASSERT(fragShaderStream.is_open(), "Couldn't open " + fragShaderStream.getFilePath() + ".");
        fragShader = fragShaderStream.readAll();
        fragShaderStream.close();

        this->rendererID = this->compileAndCheckShaders(vertShader, fragShader);
    }
    
    Shader::Shader(const std::string& vertShaderSrc, const std::string& fragShaderSrc) {
        this->rendererID = this->compileAndCheckShaders(vertShaderSrc, fragShaderSrc);
    }

    Shader::~Shader() {
        glDeleteProgram(this->rendererID);
    }

    unsigned int Shader::compileAndCheckShaders(const std::string& vertShaderSrc, const std::string& fragShaderSrc) {
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
            glGetShaderInfoLog(vertShaderID, InfoLogLength, &InfoLogLength, &VertexShaderErrorMessage[0]);

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
            glGetShaderInfoLog(fragShaderID, InfoLogLength, &InfoLogLength, &FragmentShaderErrorMessage[0]);

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
            glGetProgramInfoLog(programID, InfoLogLength, &InfoLogLength, &ProgramErrorMessage[0]);

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

        return programID;
    }
    
    void Shader::bind() const {
        glUseProgram(this->rendererID);
    }
    
    void Shader::unbind() const {
        glUseProgram(0);
    }
}
