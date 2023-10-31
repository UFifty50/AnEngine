#include "aepch.hpp"

#include "Scene/SceneSerialiser.hpp"

#include <fstream>

#include "yaml-cpp/yaml.h"

#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "entt/entt.hpp"


namespace AnEngine {
    YAML::Emitter& operator<<(YAML::Emitter& outYAML, const glm::vec3 v) {
        outYAML << YAML::Flow;
        outYAML << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return outYAML;
    }

    YAML::Emitter& operator<<(YAML::Emitter& outYAML, const glm::vec4 v) {
        outYAML << YAML::Flow;
        outYAML << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return outYAML;
    }

    SceneSerialiser::SceneSerialiser(const Ref<Scene>& scene) : scene(scene) {}

    void SceneSerialiser::serialise(const std::string& path) {
        YAML::Emitter outYAML;
        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Scene";
        outYAML << YAML::Value << scene->name;
        outYAML << YAML::Key << "Entities";
        outYAML << YAML::Value << YAML::BeginSeq;

        for (auto [entityID] : scene->entityRegistry.storage<entt::entity>().each()) {
            Entity entity = {entityID, scene.get()};
            if (!entity) return;

            serialiseEntity(outYAML, entity);
        }

        outYAML << YAML::EndSeq;
        outYAML << YAML::EndMap;

        std::ofstream outFile(path);
        outFile << outYAML.c_str();
    }

    void SceneSerialiser::serialiseBinary(const std::string& path) {
        AE_CORE_ASSERT(false, "Not Implemented");
    }

    bool SceneSerialiser::deserialise(const std::string& path) {
        YAML::Node data = YAML::LoadFile(path);
        if (!data["Scene"]) return false;

        std::string sceneName = data["Scene"].as<std::string>();
        AE_CORE_TRACE("Deserialising scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (!entities) {
            return false;
        }

        for (auto entity : entities) {
            uint64_t uuid = entity["Entity"].as<uint64_t>();  // TODO: unique entity ID
            std::string name;
            auto tagComponent = entity["TagComponent"];

            if (!tagComponent) {
                AE_CORE_WARN(
                    "Entity with ID = {0} is invalid (no tag component), skipping", uuid);
                continue;
            }

            name = tagComponent["Tag"].as<std::string>();
            Entity deserialisedEntity = scene->createEntity(name);

            if (auto transformComponent = entity["TransformComponent"]) {
                auto position = transformComponent["Position"].as<glm::vec3>();
                auto rotation = transformComponent["Rotation"].as<glm::vec3>();
                auto scale = transformComponent["Scale"].as<glm::vec3>();

                auto& tc = deserialisedEntity.getComponent<TransformComponent>();
                tc.Position = position;
                tc.Rotation = rotation;
                tc.Scale = scale;
            }

            if (auto spriteRendererComponent = entity["SpriteRendererComponent"]) {
                auto colour = spriteRendererComponent["Colour"].as<glm::vec4>();

                auto& sRC = deserialisedEntity.addComponent<SpriteRendererComponent>();
                sRC.Colour = colour;
            }

            if (auto cameraComponent = entity["CameraComponent"]) {
                auto& cc = deserialisedEntity.addComponent<CameraComponent>();
                auto cameraProps = cameraComponent["Camera"];

                cc.Camera.isPerspective =
                    cameraProps["ProjectionType"].as<std::string>() == "Perspective";
                cc.Camera.isOrthographic = !cc.Camera.isPerspective;

                if (cc.Camera.isPerspective) {
                    cc.Camera.setPerspectiveFOV(cameraProps["FOV"].as<float>());
                    cc.Camera.setPerspectiveNear(cameraProps["NearPlane"].as<float>());
                    cc.Camera.setPerspectiveFar(cameraProps["FarPlane"].as<float>());
                } else {
                    cc.Camera.setOrthographicSize(cameraProps["Size"].as<float>());
                    cc.Camera.setOrthographicNear(cameraProps["NearPlane"].as<float>());
                    cc.Camera.setOrthographicFar(cameraProps["FarPlane"].as<float>());
                    cc.FixedAspectRatio =
                        cameraProps["AspectRatioType"].as<std::string>() == "Fixed";
                }

                cc.Primary = cameraComponent["Primary"].as<bool>();
                cc.Camera.aspectRatio = cameraComponent["AspectRatio"].as<float>();

                cc.Camera.setType(cc.Camera.isPerspective ? ProjectionType::Perspective
                                                          : ProjectionType::Orthographic);
            }

            if (auto nativeScriptComponent = entity["NativeScriptComponent"]) {
                class temp : public ScriptableEntity {};

                std::string scriptName =
                    nativeScriptComponent["ScriptName"].as<std::string>();
                auto& nSC =
                    deserialisedEntity.addComponent<NativeScriptComponent>(scriptName);
                nSC.bind<temp>();
            }

            AE_CORE_TRACE("Deserialised entity with ID = {0}, name = {1}", uuid, name);
        }

        return true;
    }

    bool SceneSerialiser::deserialiseBinary(const std::string& path) {
        AE_CORE_ASSERT(false, "Not Implemented");
        return false;
    }

    void SceneSerialiser::serialiseEntity(YAML::Emitter& outYAML, Entity entity) {
        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Entity";
        outYAML << YAML::Value << "12837192831273";  // TODO: unique entity ID

        if (entity.hasComponent<TagComponent>()) {
            outYAML << YAML::Key << "TagComponent";
            outYAML << YAML::BeginMap;

            auto& tag = entity.getComponent<TagComponent>().Tag;
            outYAML << YAML::Key << "Tag" << YAML::Value << tag;

            outYAML << YAML::EndMap;
        }

        if (entity.hasComponent<TransformComponent>()) {
            outYAML << YAML::Key << "TransformComponent";
            outYAML << YAML::BeginMap;

            auto& tc = entity.getComponent<TransformComponent>();
            glm::vec3 pos = tc.Position;
            glm::vec3 rot = tc.Rotation;
            glm::vec3 scale = tc.Scale;

            outYAML << YAML::Key << "Position" << YAML::Value << pos;
            outYAML << YAML::Key << "Rotation" << YAML::Value << rot;
            outYAML << YAML::Key << "Scale" << YAML::Value << scale;

            outYAML << YAML::EndMap;
        }

        if (entity.hasComponent<SpriteRendererComponent>()) {
            outYAML << YAML::Key << "SpriteRendererComponent";
            outYAML << YAML::BeginMap;

            auto& sRC = entity.getComponent<SpriteRendererComponent>();
            outYAML << YAML::Key << "Colour" << YAML::Value << sRC.Colour;

            outYAML << YAML::EndMap;
        }

        if (entity.hasComponent<CameraComponent>()) {
            outYAML << YAML::Key << "CameraComponent";
            outYAML << YAML::BeginMap;

            auto& cc = entity.getComponent<CameraComponent>();
            outYAML << YAML::Key << "Camera" << YAML::BeginMap;

            if (cc.Camera.isPerspective) {
                outYAML << YAML::Key << "ProjectionType" << YAML::Value << "Perspective";
                outYAML << YAML::Key << "FOV";
                outYAML << YAML::Value << cc.Camera.perspectiveSettings.FOV;
                outYAML << YAML::Key << "NearPlane";
                outYAML << YAML::Value << cc.Camera.perspectiveSettings.near;
                outYAML << YAML::Key << "FarPlane";
                outYAML << YAML::Value << cc.Camera.perspectiveSettings.far;
            } else {
                outYAML << YAML::Key << "ProjectionType" << YAML::Value << "Orthographic";
                outYAML << YAML::Key << "Size" << YAML::Value
                        << cc.Camera.orthoSettings.size;
                outYAML << YAML::Key << "NearPlane";
                outYAML << YAML::Value << cc.Camera.orthoSettings.near;
                outYAML << YAML::Key << "FarPlane";
                outYAML << YAML::Value << cc.Camera.orthoSettings.far;
                outYAML << YAML::Key << "AspectRatioType";
                outYAML << YAML::Value << (cc.FixedAspectRatio ? "Fixed" : "Dynamic");
            }

            outYAML << YAML::EndMap;

            outYAML << YAML::Key << "Primary" << YAML::Value << cc.Primary;
            outYAML << YAML::Key << "AspectRatio" << YAML::Value << cc.Camera.aspectRatio;

            outYAML << YAML::EndMap;
        }

        if (entity.hasComponent<NativeScriptComponent>()) {
            outYAML << YAML::Key << "NativeScriptComponent";
            outYAML << YAML::BeginMap;

            auto& nsc = entity.getComponent<NativeScriptComponent>();
            outYAML << YAML::Key << "ScriptName";
            outYAML << YAML::Value << nsc.Name;

            outYAML << YAML::EndMap;
        }

        outYAML << YAML::EndMap;
    }
};  // namespace AnEngine
