#ifndef SCENESERIALISER_H
#define SCENESERIALISER_H

#include <glm/glm.hpp>

#include <yaml-cpp/yaml.h>

#include "Core/Core.hpp"
#include "Core/UUID.hpp"
#include "Project/Project.hpp"
#include "Project/Resources/Material.hpp"
#include "Project/Resources/Scene/Entity.hpp"
#include "Project/Resources/Scene/Scene.hpp"


namespace AnEngine {
class ProjectSerialiser {
public:
    static void SerialiseProject(const Project& project, const fs::path& projectPath);
    static Project DeserialiseProject(const fs::path& resourcePath);

    static Resource DeserialiseResource(const fs::path& rootDir, const fs::path& resourcePath);

    static Resource DeserialiseResource(const fs::path& absPath) {
        return DeserialiseResource(absPath.parent_path(), absPath.filename());
    }

    static void SerialiseResource(const Resource& resource, const fs::path& resourcePath);

private:
    static void SerialiseScene2D(const Resource& sceneRes, const fs::path& resourcePath);
    static Resource DeserialiseScene2D(const fs::path& resourcePath,
                                       const YAML::Node& resourceData);

    static void SerialiseMaterial(const Resource& matRes, const fs::path& resourcePath);
    static Resource DeserialiseMaterial(const fs::path& resourcePath,
                                        const YAML::Node& resourceData);

    static void SerialiseEntity(YAML::Emitter& outYAML, Entity entity);
};
}; // namespace AnEngine

namespace YAML {
template <>
struct convert<glm::vec3> {
    static Node encode(const glm::vec3& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec3& rhs) {
        if (!node.IsSequence() || node.size() != 3) return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        return true;
    }
};

template <>
struct convert<glm::vec4> {
    static Node encode(const glm::vec4& rhs) {
        Node node;
        node.push_back(rhs.x);
        node.push_back(rhs.y);
        node.push_back(rhs.z);
        node.push_back(rhs.w);
        node.SetStyle(EmitterStyle::Flow);
        return node;
    }

    static bool decode(const Node& node, glm::vec4& rhs) {
        if (!node.IsSequence() || node.size() != 4) return false;

        rhs.x = node[0].as<float>();
        rhs.y = node[1].as<float>();
        rhs.z = node[2].as<float>();
        rhs.w = node[3].as<float>();
        return true;
    }
};

template <>
struct convert<AnEngine::UUID> {
    static Node encode(const AnEngine::UUID& rhs) {
        Node node;
        node.push_back(rhs.toString());

        return node;
    }

    static bool decode(const Node& node, AnEngine::UUID& rhs) {
        if (!node.IsScalar()) return false;

        rhs = AnEngine::UUID::FromStr(node.as<std::string>());

        return true;
    }
};

template <>
struct convert<AnEngine::Project::MetaData> {
    static Node encode(const AnEngine::Project::MetaData& rhs) {
        Node node;
        node["Author"] = rhs.author;
        node["Version"] = rhs.version;
        node["Created"] = rhs.created;
        node["Saved"] = rhs.saved;
        node["UUID"] = rhs.uuid.toString();

        return node;
    }

    static bool decode(const Node& node, AnEngine::Project::MetaData& rhs) {
        if (!node.IsMap()) return false;

        rhs.author = node["Author"].as<std::string>();
        rhs.version = node["Version"].as<std::string>();
        rhs.created = node["Created"].as<std::string>();
        rhs.saved = node["Saved"].as<std::string>();
        rhs.uuid = node["UUID"].as<AnEngine::UUID>();

        return true;
    }
};

template <>
struct convert<AnEngine::Resource::Type> {
    static Node encode(const AnEngine::Resource::Type& rhs) {
        Node node;
        switch (rhs) {
                using enum AnEngine::Resource::Type;
            case Material: node = "Material";
                break;
            case Model: node = "Model";
                break;
            case Texture: node = "Texture";
                break;
            case Shader: node = "Shader";
                break;
            case Scene2D: node = "Scene2D";
                break;
            case Scene3D: node = "Scene3D";
                break;
            default: node = "Other";
                break;
        }

        return node;
    }

    static bool decode(const Node& node, AnEngine::Resource::Type& rhs) {
        if (!node.IsScalar()) return false;
        const auto type = node.as<std::string>();
        if (type == "Material") rhs = AnEngine::Resource::Type::Material;
        else if (type == "Model") rhs = AnEngine::Resource::Type::Model;
        else if (type == "Texture") rhs = AnEngine::Resource::Type::Texture;
        else if (type == "Shader") rhs = AnEngine::Resource::Type::Shader;
        else if (type == "Scene2D") rhs = AnEngine::Resource::Type::Scene2D;
        else if (type == "Scene3D") rhs = AnEngine::Resource::Type::Scene3D;
        else rhs = AnEngine::Resource::Type::Other;
        return true;
    }
};
}; // namespace YAML


#endif
