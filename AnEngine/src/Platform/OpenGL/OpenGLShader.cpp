#include "aepch.hpp"

#include <shaderc/shaderc.hpp>
#include <spirv_cross/spirv_cross.hpp>
#include <spirv_cross/spirv_glsl.hpp>

#include "OpenGLShader.hpp"

#include <glad/glad.h>

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <ranges>

#include "Core/Timer.hpp"
#include "File/FileStream.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "Renderer/Renderer2D.hpp"
#include "Renderer/ShaderUniform.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
namespace Utils {
    static fs::path GetCacheDirectory() { return "builtins/cache/shader/opengl"; }

    static bool CacheExists() { return exists(GetCacheDirectory()); }

    static void CreateCacheDirectory() { create_directories(GetCacheDirectory()); }

    static shaderc_shader_kind ShaderCType(GLenum type) {
        AE_PROFILE_FUNCTION()

        switch (type) {
            case GL_VERTEX_SHADER: return shaderc_glsl_vertex_shader;
            case GL_FRAGMENT_SHADER: return shaderc_glsl_fragment_shader;
            case GL_GEOMETRY_SHADER: return shaderc_glsl_geometry_shader;
            case GL_COMPUTE_SHADER: return shaderc_glsl_compute_shader;

            default:
                AE_CORE_ASSERT(false, "Unknown shader type.");
                return shaderc_glsl_infer_from_source;
        }
    }

    static std::string GLStringType(GLenum type) {
        AE_PROFILE_FUNCTION()

        switch (type) {
            case GL_VERTEX_SHADER: return "GL_VERTEX_SHADER";
            case GL_FRAGMENT_SHADER: return "GL_FRAGMENT_SHADER";
            case GL_GEOMETRY_SHADER: return "GL_GEOMETRY_SHADER";
            case GL_COMPUTE_SHADER: return "GL_COMPUTE_SHADER";

            default:
                AE_CORE_ASSERT(false, "Unknown shader type.");
                return "";
        }
    }

    // GL Shader Cache from GL Shader stage
    static std::string GLShaderGLCacheExtension(GLenum shaderType) {
        AE_PROFILE_FUNCTION()

        switch (shaderType) {
            case GL_VERTEX_SHADER: return ".glcache.glvert";
            case GL_FRAGMENT_SHADER: return ".glcache.glfrag";

            default:
                AE_CORE_ASSERT(false, "Unknown shader type.");
                return "";
        }
    }

    // VK Shader Cache from GL Shader stage
    static std::string GLShaderVKCacheExtension(GLenum shaderType) {
        AE_PROFILE_FUNCTION()

        switch (shaderType) {
            case GL_VERTEX_SHADER: return ".vkcache.glvert";
            case GL_FRAGMENT_SHADER: return ".vkcache.glfrag";

            default:
                AE_CORE_ASSERT(false, "Unknown shader type.");
                return "";
        }
    }
}; // namespace Utils

OpenGLShader::OpenGLShader(FileStreamReader& mixedShaderStream,
                           const std::string& shaderName) {
    AE_PROFILE_FUNCTION()

    if (!mixedShaderStream.isStreamOpen()) {
        AE_CORE_CRITICAL("Cannot compile as {0} is not open.",
                         mixedShaderStream.getFilePath().string());
        return;
    }

    if (!Utils::CacheExists()) Utils::CreateCacheDirectory();

    name = shaderName != "" ? shaderName : mixedShaderStream.getFileName();
    filePath = mixedShaderStream.getFilePath();

    std::string shaderText;
    mixedShaderStream.readAll(shaderText);
    std::unordered_map<GLenum, std::string> shaderSources = preProcess(shaderText);
    mixedShaderStream.close();

    {
        Timer timer;
        compileOrGetVulkanBinaries(shaderSources);
        compileOrGetOpenGLBinaries();
        for (auto&& [shaderType, data] : vulkanSPRIV) reflect(shaderType, data);
        rendererID = createProgram();
        AE_CORE_WARN("Shader compilation took {0}ms", timer.elapsedMs());
    }
}

OpenGLShader::~OpenGLShader() {
    glDeleteProgram(this->rendererID);
}

std::unordered_map<GLenum, std::string> OpenGLShader::preProcess(
    const std::string& mixedShaderSrc) {
    AE_PROFILE_FUNCTION()

    std::unordered_map<GLenum, std::string> shaderSources;

    ShaderParser parser(mixedShaderSrc);
    parser.parse();
    for (auto& [shaderType, shaderSrc] : parser.getShaders()) {
        shaderSources[shaderType] = shaderSrc;
    }

    return shaderSources;
}

void OpenGLShader::compileOrGetVulkanBinaries(
    const std::unordered_map<GLenum, std::string>& shaderSources) {
    AE_PROFILE_FUNCTION()

    GLuint program = glCreateProgram();

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_vulkan,
                                 shaderc_env_version_vulkan_1_3);
    // macro defs can go here

    constexpr bool optimise = true;
    if (optimise) options.SetOptimizationLevel(shaderc_optimization_level_performance);

    vulkanSPRIV.clear();
    for (auto&& [shaderType, source] : shaderSources) {
        fs::path cachePath = Utils::GetCacheDirectory() /
            (name + Utils::GLShaderVKCacheExtension(shaderType));

        FileStreamReader in(cachePath);

        // if file exists, read it in as code
        if (in.isStreamOpen()) {
            auto& data = vulkanSPRIV[shaderType];
            data.resize(in.getSize() / sizeof(uint32_t));
            in.readData(reinterpret_cast<char*>(data.data()), in.getSize());
        }
        // file doesnt exist, compile shader into it
        else {
            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
                source.c_str(), Utils::ShaderCType(shaderType), filePath.string().c_str(), options);

            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                AE_CORE_ERROR(module.GetErrorMessage());
                AE_CORE_ASSERT(false, "");
            }

            vulkanSPRIV[shaderType] = std::vector(module.cbegin(), module.cend());

            FileStreamWriter out(cachePath);
            std::string outFilePath = out.getFilePath().string();
            AE_CORE_ASSERT(out.isStreamOpen(), "Couldn't open file {}", outFilePath);

            auto& data = vulkanSPRIV[shaderType];
            out.writeData(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint32_t));
            out.close();
        }
    }
}

void OpenGLShader::compileOrGetOpenGLBinaries() {
    AE_PROFILE_FUNCTION()

    shaderc::Compiler compiler;
    shaderc::CompileOptions options;
    options.SetTargetEnvironment(shaderc_target_env_opengl,
                                 shaderc_env_version_opengl_4_5);
    // macro defs can go here

    constexpr bool optimise = true;
    if (optimise) options.SetOptimizationLevel(shaderc_optimization_level_performance);

    openglSPRIV.clear();
    openglSource.clear();
    for (auto&& [shaderType, spirvCode] : vulkanSPRIV) {
        fs::path cachePath = Utils::GetCacheDirectory() /
            (name + Utils::GLShaderGLCacheExtension(shaderType));

        FileStreamReader in(cachePath);
        // if file exists, read it in as code
        if (in.isStreamOpen()) {
            auto& data = openglSPRIV[shaderType];
            data.resize(in.getSize() / sizeof(uint32_t));
            in.readData(reinterpret_cast<char*>(data.data()), in.getSize());
        }
        // file doesnt exist, compile shader into it
        else {
            spirv_cross::CompilerGLSL glslCompiler(spirvCode);
            openglSource[shaderType] = glslCompiler.compile();
            auto& source = openglSource[shaderType];

            shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(
                source.c_str(), Utils::ShaderCType(shaderType), filePath.string().c_str(), options);

            if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
                AE_CORE_ERROR(module.GetErrorMessage());
                AE_CORE_ASSERT(false, "");
            }

            openglSPRIV[shaderType] =
                std::vector<uint32_t>(module.cbegin(), module.cend());

            FileStreamWriter out(cachePath);
            std::string outFilePath = out.getFilePath().string();
            AE_CORE_ASSERT(out.isStreamOpen(), "Couldn't open file {}", outFilePath);

            auto& data = openglSPRIV[shaderType];
            out.writeData(reinterpret_cast<char*>(data.data()), data.size() * sizeof(uint32_t));
            out.close();
        }
    }
}

GLuint OpenGLShader::createProgram() {
    AE_PROFILE_FUNCTION()

    GLuint program = glCreateProgram();

    std::vector<GLuint> shaderIDs;
    for (auto&& [shaderType, spirvCode] : openglSPRIV) {
        GLuint shaderID = shaderIDs.emplace_back(glCreateShader(shaderType));
        glShaderBinary(1, &shaderID, GL_SHADER_BINARY_FORMAT_SPIR_V, spirvCode.data(),
                       static_cast<GLsizei>(spirvCode.size()) * sizeof(uint32_t));
        glSpecializeShader(shaderID, "main", 0, nullptr, nullptr);
        glAttachShader(program, shaderID);
    }

    glLinkProgram(program);

    GLint isLinked;
    glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
        GLint len;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);

        std::vector<GLchar> infoLog(len);
        glGetProgramInfoLog(program, len, &len, infoLog.data());
        AE_CORE_ERROR("Shader linking failed ({0}):\n{1}", filePath.string(), infoLog.data());

        glDeleteProgram(program);

        for (auto id : shaderIDs)
            glDeleteShader(id);
    }

    for (auto id : shaderIDs) {
        glDetachShader(program, id);
        glDeleteShader(id);
    }

    AE_CORE_DEBUG("Shader {0} compiled successfully.", name);

    return program;
}

void OpenGLShader::reflect(GLenum shaderType, const std::vector<uint32_t>& spirvCode) {
    AE_PROFILE_FUNCTION()

    spirv_cross::Compiler compiler(spirvCode);
    spirv_cross::ShaderResources resources = compiler.get_shader_resources();

    AE_CORE_TRACE("OpenGLShader::reflect - {0} {1}", Utils::GLStringType(shaderType),
                  filePath.string());
    AE_CORE_TRACE("    {0} uniform buffer(s)", resources.uniform_buffers.size());
    AE_CORE_TRACE("    {0} resource(s)", resources.sampled_images.size());

    if (resources.uniform_buffers.size() > 0)
        AE_CORE_TRACE("Uniform buffers:");

    for (const auto& res : resources.uniform_buffers) {
        const auto& bufferType = compiler.get_type(res.base_type_id);
        size_t bufferSize = compiler.get_declared_struct_size(bufferType);
        size_t binding = compiler.get_decoration(res.id, spv::DecorationBinding);
        size_t memberCount = bufferType.member_types.size();

        AE_CORE_TRACE("  {0}", res.name);
        AE_CORE_TRACE("    Size = {0}", bufferSize);
        AE_CORE_TRACE("    Binding = {0}", binding);
        AE_CORE_TRACE("    Members = {0}", memberCount);
    }
}

void OpenGLShader::bind() const {
    glUseProgram(rendererID);
}

void OpenGLShader::unbind() const {
    glUseProgram(0);
}

void OpenGLShader::uploadUniform(const std::string& name, std::any uniform) {
    AE_PROFILE_FUNCTION()

    constexpr uint32_t maxTextureSlots = Renderer2D::MaterialBatch::maxTextureSlots;

    GLint location = glGetUniformLocation(rendererID, name.c_str());
    if (location == -1) {
        AE_CORE_ERROR("Uniform {0} doesn't exist.", name);
        return;
    }

    // integral types
    if (uniform.type() == typeid(int32_t)) {
        glUniform1i(location, std::any_cast<int32_t>(uniform));
    }
    else if (uniform.type() == typeid(uint32_t)) {
        glUniform1ui(location, std::any_cast<uint32_t>(uniform));
    }
    else if (uniform.type() == typeid(bool)) {
        glUniform1i(location, std::any_cast<bool>(uniform));
    }
    else if (uniform.type() == typeid(float)) {
        glUniform1f(location, std::any_cast<float>(uniform));
    }
    else if (uniform.type() == typeid(double)) {
        glUniform1d(location, std::any_cast<double>(uniform));
    }

    // 2D vectors
    else if (uniform.type() == typeid(glm::vec2)) {
        auto vec = std::any_cast<glm::vec2>(uniform);
        glUniform2f(location, vec.x, vec.y);
    }
    else if (uniform.type() == typeid(glm::ivec2)) {
        auto vec = std::any_cast<glm::ivec2>(uniform);
        glUniform2i(location, vec.x, vec.y);
    }
    else if (uniform.type() == typeid(glm::uvec2)) {
        auto vec = std::any_cast<glm::uvec2>(uniform);
        glUniform2ui(location, vec.x, vec.y);
    }
    else if (uniform.type() == typeid(glm::dvec2)) {
        auto vec = std::any_cast<glm::dvec2>(uniform);
        glUniform2d(location, vec.x, vec.y);
    }
    else if (uniform.type() == typeid(glm::bvec2)) {
        auto vec = std::any_cast<glm::bvec2>(uniform);
        glUniform2i(location, vec.x, vec.y);
    }

    // 3D vectors
    else if (uniform.type() == typeid(glm::vec3)) {
        auto vec = std::any_cast<glm::vec3>(uniform);
        glUniform3f(location, vec.x, vec.y, vec.z);
    }
    else if (uniform.type() == typeid(glm::ivec3)) {
        auto vec = std::any_cast<glm::ivec3>(uniform);
        glUniform3i(location, vec.x, vec.y, vec.z);
    }
    else if (uniform.type() == typeid(glm::uvec3)) {
        auto vec = std::any_cast<glm::uvec3>(uniform);
        glUniform3ui(location, vec.x, vec.y, vec.z);
    }
    else if (uniform.type() == typeid(glm::dvec3)) {
        auto vec = std::any_cast<glm::dvec3>(uniform);
        glUniform3d(location, vec.x, vec.y, vec.z);
    }
    else if (uniform.type() == typeid(glm::bvec3)) {
        auto vec = std::any_cast<glm::bvec3>(uniform);
        glUniform3i(location, vec.x, vec.y, vec.z);
    }

    // 4D vectors
    else if (uniform.type() == typeid(glm::vec4)) {
        auto vec = std::any_cast<glm::vec4>(uniform);
        glUniform4f(location, vec.r, vec.g, vec.b, vec.a);
    }
    else if (uniform.type() == typeid(glm::ivec4)) {
        auto vec = std::any_cast<glm::ivec4>(uniform);
        glUniform4i(location, vec.r, vec.g, vec.b, vec.a);
    }
    else if (uniform.type() == typeid(glm::uvec4)) {
        auto vec = std::any_cast<glm::uvec4>(uniform);
        glUniform4ui(location, vec.r, vec.g, vec.b, vec.a);
    }
    else if (uniform.type() == typeid(glm::dvec4)) {
        auto vec = std::any_cast<glm::dvec4>(uniform);
        glUniform4d(location, vec.r, vec.g, vec.b, vec.a);
    }
    else if (uniform.type() == typeid(glm::bvec4)) {
        auto vec = std::any_cast<glm::bvec4>(uniform);
        glUniform4i(location, vec.r, vec.g, vec.b, vec.a);
    }


    // Matrices
    else if (uniform.type() == typeid(glm::mat2)) {
        glUniformMatrix2fv(location, 1, GL_FALSE,
                           value_ptr(std::any_cast<glm::mat2>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat2x3)) {
        glUniformMatrix2x3fv(location, 1, GL_FALSE,
                             value_ptr(std::any_cast<glm::mat4x2>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat2x4)) {
        glUniformMatrix2x4fv(location, 1, GL_FALSE,
                             value_ptr(std::any_cast<glm::mat4x3>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat3)) {
        glUniformMatrix3fv(location, 1, GL_FALSE,
                           value_ptr(std::any_cast<glm::mat3>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat3x2)) {
        glUniformMatrix3x2fv(location, 1, GL_FALSE,
                             value_ptr(std::any_cast<glm::mat4x2>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat3x4)) {
        glUniformMatrix3x4fv(location, 1, GL_FALSE,
                             value_ptr(std::any_cast<glm::mat4x2>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat4)) {
        glUniformMatrix4fv(location, 1, GL_FALSE,
                           value_ptr(std::any_cast<glm::mat4>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat4x2)) {
        glUniformMatrix4x2fv(location, 1, GL_FALSE,
                             value_ptr(std::any_cast<glm::mat4x2>(uniform)));
    }
    else if (uniform.type() == typeid(glm::mat4x3)) {
        glUniformMatrix4x3fv(location, 1, GL_FALSE,
                             value_ptr(std::any_cast<glm::mat4x3>(uniform)));
    }

    // Textures
    else if (uniform.type() == typeid(Sampler2D)) {
        uint32_t slot = std::any_cast<Sampler2D>(uniform).slot;
        glUniform1i(location, slot);
    }
    else if (uniform.type() == typeid(std::array<Sampler2D, maxTextureSlots>)) {
        auto samplerArray = std::any_cast<std::array<Sampler2D, maxTextureSlots>>(uniform);
        int32_t data[maxTextureSlots]{};

        for (uint32_t i = 0; i < maxTextureSlots; i++) { data[i] = samplerArray[i].slot; }

        glUniform1iv(location, maxTextureSlots, data);
    }

    else { AE_CORE_ASSERT(false, "Unknown uniform type."); }
}


ShaderParser::ShaderParser(const std::string& mixedShaderSrc)
    : mixedShaderSrc(mixedShaderSrc), shaderType(GL_FALSE) {}

void ShaderParser::parse() {
    AE_PROFILE_FUNCTION()

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

            if (shaderTypeStr.empty()) { AE_CORE_ASSERT(false, "Shader type must be specified."); }

            switch (HashedType(shaderTypeStr)) {
                case VERTEX: shaderType = GL_VERTEX_SHADER;
                    break;
                case FRAGMENT: shaderType = GL_FRAGMENT_SHADER;
                    break;
                case GEOMETRY: shaderType = GL_GEOMETRY_SHADER;
                    break;
                case COMPUTE: shaderType = GL_COMPUTE_SHADER;
                    break;
                default:
                    AE_CORE_ASSERT(false, "Unknown shader type: \"{0}\"", shaderTypeStr);
                    break;
            }

            if (shaders.contains(shaderType)) {
                AE_CORE_ASSERT(false, "{0} Shader already defined.", shaderTypeStr);
            }
        }
        else { shaders[shaderType] += line + "\n"; }
    }
}

std::unordered_map<GLenum, std::string> ShaderParser::getShaders() const {
    AE_PROFILE_FUNCTION()

    return shaders;
}
} // namespace AnEngine
