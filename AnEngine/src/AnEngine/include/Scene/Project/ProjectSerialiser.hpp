#ifndef SCENESERIALISER_H
#define SCENESERIALISER_H

#include <glm/glm.hpp>

#include <yaml-cpp/yaml.h>

#include "Core/Core.hpp"
#include "Core/UUID.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Project/Project.hpp"
#include "Scene/Scene.hpp"
#include "Texture/Material.hpp"


namespace AnEngine {
    class ProjectSerialiser {
    public:
        static Project openProject(const fs::path& path = "");

        static void serialiseProject(const Project& project, const fs::path& path = "");
        static Project deserialiseProject(const fs::path& path = "");

        static void serialiseScene(const std::string& path, const Ref<Scene>& scene);
        static bool deserialiseScene(const std::string& path, const Ref<Scene>& scene);

        static void serialiseMaterial(const std::string& path, Material material);
        static Material deserialiseMaterial(const fs::path& path);

    private:
        static void serialiseEntity(YAML::Emitter& outYAML, Entity entity);
    };
};  // namespace AnEngine

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

    /*template <>
    struct convert<AnEngine::UUID> {
        static Node encode(const AnEngine::UUID& rhs) {
            Node node;
            node.push_back((std::string)rhs);

            return node;
        }

        static bool decode(const Node& node, AnEngine::UUID& rhs) {
            if (!node.IsScalar()) return false;

            rhs.id[0] = std::stoi(node.as<std::string>().substr(0, 8), nullptr, 16);
            rhs.id[1] = std::stoi(node.as<std::string>().substr(10, 8), nullptr, 16);

            return true;
        }
    };*/
};  // namespace YAML


#endif
