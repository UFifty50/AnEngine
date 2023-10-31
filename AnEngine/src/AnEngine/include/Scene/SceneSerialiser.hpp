#ifndef SCENESERIALISER_H
#define SCENESERIALISER_H

#include <glm/glm.hpp>

#include <yaml-cpp/yaml.h>

#include "Scene/Entity.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine {
    class SceneSerialiser {
    public:
        SceneSerialiser(const Ref<Scene>& scene);

        void serialise(const std::string& path);
        void serialiseBinary(const std::string& path);

        bool deserialise(const std::string& path);
        bool deserialiseBinary(const std::string& path);

    private:
        void serialiseEntity(YAML::Emitter& outYAML, Entity entity);

        Ref<Scene> scene;
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
};  // namespace YAML


#endif
