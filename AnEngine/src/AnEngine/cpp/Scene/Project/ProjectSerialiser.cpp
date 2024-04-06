#include "aepch.hpp"

#define NOMINMAX
#include <entt/entt.hpp>

#include <yaml-cpp/yaml.h>

#include <fstream>
#include <string>

#include "Core/UUID.hpp"
#include "Globals.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"
#include "Scene/Project/Project.hpp"
#include "Scene/Project/ProjectSerialiser.hpp"
#include "Texture/Material.hpp"
#include "Texture/Texture2D.hpp"

/// TODO: Projects
/// DOING  - Core Scene YAML
/// DOING  - Directory structure
///        - Every file item has a Meta file for its UUID
/// DOING  - Materials are referenced by UUID
///        - Binary data is referenced by UUID


constexpr uint32_t hs(const char* str) {
    uint32_t hash = 2166136261;
    while (*str) {
        hash ^= *str++;
        hash *= 16777619;
    }
    return hash;
}

namespace AnEngine {
    YAML::Emitter& operator<<(YAML::Emitter& outYAML, const glm::vec3 v) {
        AE_PROFILE_FUNCTION()

        outYAML << YAML::Flow;
        outYAML << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return outYAML;
    }

    YAML::Emitter& operator<<(YAML::Emitter& outYAML, const glm::vec4 v) {
        AE_PROFILE_FUNCTION()

        outYAML << YAML::Flow;
        outYAML << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return outYAML;
    }

    YAML::Emitter& operator<<(YAML::Emitter& outYAML, const Ref<Texture2D> t) {
        AE_PROFILE_FUNCTION()

        if (t)
            outYAML << t->getPath().string();
        else
            outYAML << "None";

        return outYAML;
    }

    YAML::Emitter& operator<<(YAML::Emitter& outYAML, const UUID uuid) {
        AE_PROFILE_FUNCTION();

        outYAML << (std::string)uuid;
        return outYAML;
    }

    // example YAML project file
    // name: "Crank"
    // meta:
    //   - author: "AnEngine"
    //   - version: "0.0.1"
    //   - created: "2021-10-01"
    //   - saved: "2021-10-01"
    //   - uuid: "ffffffff-ffffffff"
    // root:
    //   - files:
    //       - name: "material.yml"
    //         path: "materials/material.yml"
    //         type: "Material"
    //   - directories:
    //       - name: "models"
    //         files:
    //           - name: "model.obj"
    //             path: "models/model.obj"
    //             type: "Model"
    //       - name: "textures"
    //         files:
    //           - name: "texture.png"
    //             path: "textures/texture.png"
    //             type: "Texture"
    //         directories:
    //           - name: "normal"
    //             files:
    //               - name: "normal.png"
    //                 path: "textures/normal/normal.png"
    //                 type: "Texture"

    Project ProjectSerialiser::openProject(const fs::path& path) {
        AE_PROFILE_FUNCTION()

        if (!fs::exists(path)) {
            AE_CORE_ERROR("Project path does not exist");
            return;
        }

        if (!path.has_extension() || path.extension() != ".aeproj") {
            AE_CORE_ERROR("Project path is not a valid project file");
            return;
        }

        Project project = ProjectSerialiser::deserialiseProject(path);

        // Load materials
        for (auto file : project.root) {
            if (file.type != DirectoryEntry::File) continue;

            File file = static_cast<File>(file);
            if (file.type != FileType::Material) continue;

            Material material = ProjectSerialiser::deserialiseMaterial(file.path);
            project.materials[material.uuid] = material;

            AE_CORE_TRACE("Loaded material '{0}' with UUID = {1}", material.name,
                          (std::string)material.uuid);
        }
    }

    void ProjectSerialiser::serialiseProject(const Project& project, const fs::path& path) {}

    Project ProjectSerialiser::deserialiseProject(const fs::path& path) {
        AE_PROFILE_FUNCTION()

        Project project;
        YAML::Node data = YAML::LoadFile(path.string());

        project.path = path;
        project.name = data["name"].as<std::string>();
        project.meta = data["meta"].as<Project::MetaData>();

        auto root = data["root"];

        std::function<void(const YAML::Node&, Directory&)> iterateDirectory =
            [&](const YAML::Node& node, Directory& directory) {
                auto files = node["files"];
                auto directories = node["directories"];

                for (auto file : files) {
                    UUID uuid = file.first.as<UUID>();
                    fs::path path = file.second["path"].as<std::string>();
                    std::string name = file.second["name"].as<std::string>();
                    std::string type = file.second["type"].as<std::string>();

                    FileType fileType = FileType::Other;
                    if (type == "Material") {
                        fileType = FileType::Material;
                    } else if (type == "Texture") {
                        fileType = FileType::Texture;
                    } else if (type == "Model") {
                        fileType = FileType::Model;
                    }

                    directory.files.insert(File(uuid, name, fileType, path));
                }

                for (auto dir : directories) {
                    UUID uuid = dir.first.as<UUID>();
                    std::string name = dir.second["name"].as<std::string>();
                    Directory subDir;
                    iterateDirectory(dir.second, subDir);

                    directory.directories.insert(subDir);
                }
            };

        iterateDirectory(root, project.root);

        return project;
    }

    void ProjectSerialiser::serialiseScene(const std::string& path, const Ref<Scene>& scene) {
        AE_PROFILE_FUNCTION()

        YAML::Emitter outYAML;
        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Scene";
        outYAML << YAML::Value << scene->getName();
        outYAML << YAML::Key << "Entities";
        outYAML << YAML::Value << YAML::BeginSeq;

        for (auto [entityID] : scene->getRegistry().storage<entt::entity>().each()) {
            Entity entity = {entityID, scene.get()};
            if (!entity) return;

            ProjectSerialiser::serialiseEntity(outYAML, entity);
        }

        outYAML << YAML::EndSeq;
        outYAML << YAML::EndMap;

        std::ofstream outFile(path);
        outFile << outYAML.c_str();
    }

    bool ProjectSerialiser::deserialiseScene(const std::string& path,
                                             const Ref<Scene>& scene) {
        AE_PROFILE_FUNCTION()

        YAML::Node data = YAML::LoadFile(path);
        try {
            if (!data["Scene"]) return false;
        } catch (std::exception e) {
            AE_CORE_ERROR("Error deserializing scene: {0}", e.what());
            return false;
        }

        std::string sceneName = data["Scene"].as<std::string>();
        AE_CORE_TRACE("Deserialising scene '{0}'", sceneName);

        auto entities = data["Entities"];
        if (!entities) {
            return false;
        }

        for (auto entity : entities) {
            std::string uuidStr = entity["Entity"].as<std::string>();
            auto tagComponent = entity["TagComponent"];

            if (!tagComponent) {
                AE_CORE_WARN("Entity with ID = {0} is invalid (no tag component), skipping",
                             uuidStr);
                continue;
            }

            std::string name = tagComponent["Tag"].as<std::string>();
            UUID uuid = UUID::fromStr(uuidStr);

            Entity deserialisedEntity = scene->createEntityWithUUID(name, uuid);

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
                auto material = spriteRendererComponent["Material"];

                auto& sRC = deserialisedEntity.addComponent<SpriteRendererComponent>();
                sRC.Mat.colour = material["Colour"].as<glm::vec4>();

                if (material["Texture"].as<std::string>() != "None")
                    sRC.Mat.texture = Texture2D::create(material["Texture"].as<std::string>());
            }

            if (auto cameraComponent = entity["CameraComponent"]) {
                auto& cc = deserialisedEntity.addComponent<CameraComponent>();
                auto cameraProps = cameraComponent["Camera"];

                cc.Camera.isPerspective =
                    cameraProps["ProjectionType"].as<std::string>() == "Perspective";


                if (cc.Camera.isPerspective) {
                    cc.Camera.updateSpec(CameraSpec3D::Feild::FOVorSize,
                                         cameraProps["FOV"].as<float>());
                } else {
                    cc.Camera.updateSpec(CameraSpec3D::Feild::FOVorSize,
                                         cameraProps["Size"].as<float>());
                    cc.FixedAspectRatio =
                        cameraProps["AspectRatioType"].as<std::string>() == "Fixed";
                }

                cc.Camera.updateSpec(CameraSpec3D::Feild::NearPlane,
                                     cameraProps["NearPlane"].as<float>());
                cc.Camera.updateSpec(CameraSpec3D::Feild::FarPlane,
                                     cameraProps["FarPlane"].as<float>());

                cc.Primary = cameraComponent["Primary"].as<bool>();
                cc.Camera.aspectRatio = cameraComponent["AspectRatio"].as<float>();

                cc.Camera.changeProjectionType(cc.Camera.isPerspective
                                                   ? ProjectionType::Perspective
                                                   : ProjectionType::Orthographic);
            }

            if (auto nativeScriptComponent = entity["NativeScriptComponent"]) {
                class temp : public ScriptableEntity {};

                std::string scriptName = nativeScriptComponent["ScriptName"].as<std::string>();
                auto& nSC = deserialisedEntity.addComponent<NativeScriptComponent>(scriptName);
                nSC.bind<temp>();
            }

            AE_CORE_TRACE("Deserialised entity with name = {0}, ID = {1}", name,
                          (std::string)uuid);
        }

        return true;
    }

    void ProjectSerialiser::serialiseMaterial(const std::string& path, Material material) {
        YAML::Emitter outYAML;
        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Material";
        outYAML << YAML::Value << material.name;

        outYAML << YAML::Key << "UUID";
        outYAML << YAML::Value << material.uuid;

        outYAML << YAML::Key << "Attributes";
        outYAML << YAML::Value << YAML::BeginSeq;

        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Colour" << YAML::Value << material.colour;
        outYAML << YAML::EndMap;

        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Texture" << YAML::Value;

        if (material.texture)
            outYAML << material.texture->getPath().string();
        else
            outYAML << "None";
        outYAML << YAML::EndMap;

        outYAML << YAML::EndSeq;
        outYAML << YAML::EndMap;

        std::ofstream outFile(path);
        outFile << outYAML.c_str();
    }

    Material ProjectSerialiser::deserialiseMaterial(const fs::path& path) {
        AE_PROFILE_FUNCTION()

        YAML::Node data;
        try {
            data = YAML::LoadFile(path.string());
        } catch (std::exception e) {
            AE_CORE_ERROR("Error deserializing material: {0}", e.what());
            return Material();
        }

        if (!data["Material"]) {
            AE_CORE_ERROR("Invalid material!");
            return Material();
        }

        std::string materialName = data["Material"].as<std::string>();
        std::string uuidStr = data["UUID"].as<std::string>();
        UUID uuid = UUID::fromStr(uuidStr);

        Material deserialisedMaterial = Material(materialName, uuid);

        AE_CORE_TRACE("Deserialising Material with name = {0}, UUID = {1}", materialName,
                      uuidStr);

        auto attributes = data["Attributes"];
        if (!attributes) {
            AE_CORE_ERROR("Invalid material!");
            return Material();
        }

        for (auto attribute : attributes) {
            if (auto colour = attribute["Colour"]) {
                deserialisedMaterial.colour = colour.as<glm::vec4>();
            }

            if (auto texture = attribute["Texture"]) {
                if (texture.as<std::string>() != "None")
                    deserialisedMaterial.texture =
                        Texture2D::create(texture.as<std::string>());
            }
        }


        AE_CORE_TRACE("Deserialised Material with name = {0}, UUID = {1}", materialName,
                      uuidStr);

        return deserialisedMaterial;
    }

    void ProjectSerialiser::serialiseEntity(YAML::Emitter& outYAML, Entity entity) {
        AE_PROFILE_FUNCTION()

        if (!entity.hasComponent<IDComponent>()) {
            AE_CORE_ERROR("Entity doesn't have UUID, aborting");
            return;
        }

        outYAML << YAML::BeginMap;
        outYAML << YAML::Key << "Entity";
        outYAML << YAML::Value << entity.getComponent<IDComponent>().uuid;

        if (entity.hasComponent<TagComponent>()) {
            outYAML << YAML::Key << "TagComponent";
            outYAML << YAML::BeginMap;

            auto& tag = entity.getComponent<TagComponent>().Tag;
            outYAML << YAML::Key << "Tag" << YAML::Value << tag.data();

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
            // Unity uses YAML for materials, and each texture is just an ID for the assets
            // manager to use
            outYAML << YAML::Key << "Material" << YAML::Value << sRC.Mat.uuid;
            outYAML << YAML::EndMap;

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
                outYAML << YAML::Key << "Size" << YAML::Value << cc.Camera.orthoSettings.size;
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
