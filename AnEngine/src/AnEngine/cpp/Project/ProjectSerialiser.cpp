#include "aepch.hpp"

#define NOMINMAX
#include <entt/entt.hpp>

#include "Project/ProjectSerialiser.hpp"

#include <yaml-cpp/yaml.h>

#include <string>

#include "Globals.hpp"
#include "Core/UUID.hpp"
#include "Project/Project.hpp"
#include "Project/Resources/Material.hpp"
#include "Project/Resources/Scene/Components.hpp"
#include "Project/Resources/Scene/Entity.hpp"
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

    if (t) outYAML << t->getPath().string();
    else outYAML << "None";

    return outYAML;
}

YAML::Emitter& operator<<(YAML::Emitter& outYAML, const UUID uuid) {
    AE_PROFILE_FUNCTION();

    outYAML << uuid.toString();
    return outYAML;
}


// example YAML project file
// name: "Crank"
// 3D: true
// activeScene: "00000000-00000000"
// meta:
//   - author: "AnEngine"
//   - version: "0.0.1"
//   - created: "2021-10-01"
//   - saved: "2021-10-01"
//   - uuid: "ffffffff-ffffffff"
// root:
//   - rootDir: "C:\\Users\\AUser\\Projects\\Project1\\"
//   - assets:
//       - "materials/material1.aeres"
//       - "models/model1.aeres"
//       - "textures/texture1.aeres"
//       - "textures/normal/normal1.aeres"


void ProjectSerialiser::SerialiseProject(const Project& project, const fs::path& projectPath) {
    AE_PROFILE_FUNCTION()
    YAML::Emitter outYAML;
    outYAML << YAML::BeginMap;
    outYAML << YAML::Key << "Name";
    outYAML << YAML::Value << project.name;

    outYAML << YAML::Key << "3D";
    outYAML << YAML::Value << project.is3D;

    outYAML << YAML::Key << "ActiveScene";
    outYAML << YAML::Value << project.activeSceneID;

    outYAML << YAML::Key << "Meta";
    outYAML << YAML::Value << YAML::BeginMap;
    outYAML << YAML::Key << "Author";
    outYAML << YAML::Value << project.meta.author;
    outYAML << YAML::Key << "Version";
    outYAML << YAML::Value << project.meta.version;
    outYAML << YAML::Key << "Created";
    outYAML << YAML::Value << project.meta.created;
    outYAML << YAML::Key << "Saved";
    outYAML << YAML::Value << project.meta.saved;
    outYAML << YAML::Key << "UUID";
    outYAML << YAML::Value << project.meta.uuid;
    outYAML << YAML::EndMap;

    outYAML << YAML::Key << "Root";
    outYAML << YAML::Value << YAML::BeginMap;

    outYAML << YAML::Key << "RootDir";
    outYAML << YAML::Value << project.rootPathAbs.string();

    outYAML << YAML::Key << "Assets";
    outYAML << YAML::Value << YAML::BeginSeq;
    for (const auto& resourceRelPath : project.resourceRelPaths | ranges::views::values) {
        outYAML << resourceRelPath.string();
    }
    outYAML << YAML::EndSeq;
    outYAML << YAML::EndMap;
    outYAML << YAML::EndMap;

    FileStreamWriter outFile(projectPath);
    outFile.writeString(outYAML.c_str());
}

Project ProjectSerialiser::DeserialiseProject(const fs::path& resourcePath) {
    AE_PROFILE_FUNCTION()

    if (!exists(resourcePath)) {
        AE_CORE_ERROR("Project path does not exist");
        return Project();
    }

    if (!resourcePath.has_extension() || resourcePath.extension() != ".aeproj") {
        AE_CORE_ERROR("Project path is not a valid project file");
        return Project();
    }

    Project project;
    YAML::Node data = YAML::LoadFile(resourcePath.string());

    // project.rootPathAbs = path;
    project.name = data["Name"].as<std::string>();
    project.is3D = data["3D"].as<bool>();
    project.activeSceneID = data["ActiveScene"].as<UUID>();
    project.meta = data["Meta"].as<Project::MetaData>();

    auto root = data["Root"];
    project.rootPathAbs = root["RootDir"].as<std::string>();

    for (auto assetRelPath : root["Assets"]) {
        Resource res = DeserialiseResource(project.rootPathAbs, assetRelPath.as<std::string>());
        project.resources[res.uuid] = res;
        project.resourceRelPaths[res.uuid] = assetRelPath.as<std::string>();
    }

    return project;

    /*
     * std::function<void(const YAML::Node&, Directory&)> iterateDirectory =
     *     [&](const YAML::Node& node, Directory& directory) {
     *     auto files = node["files"];
     *     auto directories = node["directories"];
     * 
     *     for (auto file : files) {
     *         auto uuid = file.first.as<UUID>();
     *         fs::path path = file.second["path"].as<std::string>();
     *         auto name = file.second["name"].as<std::string>();
     *         auto type = file.second["type"].as<std::string>();
     * 
     *         auto fileType = FileType::Other;
     *         if (type == "Material") { fileType = FileType::Material; } else if (
     *             type == "Texture") { fileType = FileType::Texture; } else if (
     *             type == "Model") { fileType = FileType::Model; }
     * 
     *         directory.files.insert(File(uuid, name, fileType, path));
     *     }
     * 
     *     for (auto dir : directories) {
     *         auto uuid = dir.first.as<UUID>();
     *         auto name = dir.second["name"].as<std::string>();
     *         Directory subDir;
     *         iterateDirectory(dir.second, subDir);
     * 
     *         directory.directories.insert(subDir);
     *     }
     * };
     * 
     * iterateDirectory(root, project.root);
     */
}


//enum class Type { Other, Material, Model, Texture, Shader, Scene2D, Scene3D };
//
//Type type;
//UUID uuid;
//std::string name;

// type: "Material"
// uuid: "12345678-90abcdef"
// name: "Material1"
Resource ProjectSerialiser::DeserialiseResource(const fs::path& rootDir,
                                                const fs::path& resourcePath) {
    AE_PROFILE_FUNCTION()

    const fs::path fullPath = rootDir / resourcePath;
    if (!exists(fullPath)) {
        AE_CORE_ERROR("Resource path \"{}\" does not exist", fullPath.string());
        return Resource{};
    }

    if (!fullPath.has_extension() || fullPath.extension() != ".aeres") {
        AE_CORE_ERROR("Resource path \"{}\" is not a valid resource file", fullPath.string());
        return Resource{};
    }

    YAML::Node data = YAML::LoadFile(fullPath.string());

    switch (const auto type = data["Type"].as<Resource::Type>()) {
            using enum Resource::Type;

        case Material: return DeserialiseMaterial(rootDir, data);

        // TODO: Jank as fuck, find a better way
        case Scene2D: return DeserialiseScene2D(fullPath, data);
        case Scene3D:
        case Texture:
        case Model:
        case Shader:
        case Other: AE_CORE_WARN("Cannot serialise resource of type {}",
                                 Resource::TypeToString(type));
    }

    AE_CORE_ERROR("Resource type not recognised for path \"{}\"", fullPath.string());
    return Resource{};
}

void ProjectSerialiser::SerialiseResource(const Resource& resource, const fs::path& resourcePath) {
    AE_PROFILE_FUNCTION()

    switch (resource.type) {
            using enum Resource::Type;
        case Material: SerialiseMaterial(resource, resourcePath);
        case Scene2D: SerialiseScene2D(resource, resourcePath);

        case Scene3D:
        case Texture:
        case Model:
        case Shader:
        case Other: AE_CORE_WARN("Cannot serialise resource of type {}", resource.typeToString());
    }
}

void ProjectSerialiser::SerialiseScene2D(const Resource& sceneRes, const fs::path& resourcePath) {
    AE_PROFILE_FUNCTION()

    const Ref<Scene> scene = std::reinterpret_pointer_cast<Scene>(MakeRef<Resource>(sceneRes));

    YAML::Emitter outYAML;
    outYAML << YAML::BeginMap;
    outYAML << YAML::Key << "Type";
    outYAML << YAML::Value << Resource::TypeToString(sceneRes.type);

    outYAML << YAML::Key << "UUID";
    outYAML << YAML::Value << scene->uuid;

    outYAML << YAML::Key << "Name";
    outYAML << YAML::Value << scene->name;

    outYAML << YAML::Key << "Entities";
    outYAML << YAML::Value << YAML::BeginSeq;

    for (const auto& [entityID] : scene->getRegistry()->storage<entt::entity>().each()) {
        const Entity entity = {entityID, scene.get()};
        if (!entity) return;

        SerialiseEntity(outYAML, entity);
    }

    outYAML << YAML::EndSeq;
    outYAML << YAML::EndMap;

    FileStreamWriter outFile(resourcePath);
    outFile.writeString(outYAML.c_str());
}

Resource ProjectSerialiser::DeserialiseScene2D(const fs::path& resourcePath,
                                               const YAML::Node& resourceData) {
    AE_PROFILE_FUNCTION()

    YAML::Node data = YAML::LoadFile(resourcePath.string());

    if (!data) {
        AE_CORE_ERROR("Error deserializing scene: {0}\nCouldn't open resource.",
                      resourcePath.string());
        return Resource{};
    }

    Resource sceneRes = Scene2D::Create();
    sceneRes.type = Resource::Type::Scene2D;
    sceneRes.uuid = UUID::FromStr(resourceData["UUID"].as<std::string>());
    sceneRes.name = resourceData["Name"].as<std::string>();

    Ref<Scene> scene = std::reinterpret_pointer_cast<Scene>(Ref<Resource>(&sceneRes));

    AE_CORE_TRACE("Deserialising scene '{0}'", sceneRes.name);

    auto entities = data["Entities"];
    for (auto entity : entities) {
        auto uuid = entity["Entity"].as<UUID>();
        auto tagComponent = entity["TagComponent"];

        if (!tagComponent) {
            AE_CORE_WARN("Entity with ID = {0} is invalid (no tag component), skipping",
                         uuid.toString());
            continue;
        }

        auto name = tagComponent["Tag"].as<std::string>();
        Entity& deserialisedEntity = scene->createEntityWithUUID(name, uuid);

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

            if (material["Texture"].as<std::string>() !=
                "None")
                sRC.Mat.texture = Texture2D::create(
                    material["Texture"].as<std::string>());
        }

        if (auto cameraComponent = entity["CameraComponent"]) {
            auto& cc = deserialisedEntity.addComponent<CameraComponent>();
            auto cameraProps = cameraComponent["Camera"];

            cc.Camera.isPerspective =
                cameraProps["ProjectionType"].as<std::string>() == "Perspective";


            if (cc.Camera.isPerspective) {
                cc.Camera.updateSpec(CameraSpec3D::Feild::FOVorSize,
                                     cameraProps["FOV"].as<float>());
            }
            else {
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
            // class temp : public ScriptableEntity {};

            // std::string scriptName =
            // nativeScriptComponent["ScriptName"].as<std::string>(); auto& nSC =
            // deserialisedEntity.addComponent<NativeScriptComponent>(scriptName);
            // nSC.bind<temp>();
        }

        AE_CORE_TRACE("Deserialised entity with name = {0}, ID = {1}", name, uuid.toString());
    }

    return sceneRes;
}

void ProjectSerialiser::SerialiseMaterial(const Resource& matRes, const fs::path& resourcePath) {
    const auto& material = static_cast<const Material&>(matRes);

    YAML::Emitter outYAML;
    outYAML << YAML::BeginMap;
    outYAML << YAML::Key << "Type";
    outYAML << YAML::Value << Resource::TypeToString(matRes.type);

    outYAML << YAML::Key << "UUID";
    outYAML << YAML::Value << material.uuid;

    outYAML << YAML::Key << "Name";
    outYAML << YAML::Value << material.name;

    outYAML << YAML::Key << "Attributes";
    outYAML << YAML::Value << YAML::BeginSeq;

    outYAML << YAML::BeginMap;
    outYAML << YAML::Key << "Colour" << YAML::Value << material.colour;
    outYAML << YAML::EndMap;

    outYAML << YAML::BeginMap;
    outYAML << YAML::Key << "Texture" << YAML::Value;

    if (material.texture) outYAML << material.texture->getPath().string();
    else outYAML << "None";
    outYAML << YAML::EndMap;

    outYAML << YAML::EndSeq;
    outYAML << YAML::EndMap;

    FileStreamWriter outFile(resourcePath);
    outFile.writeString(outYAML.c_str());
}

Resource ProjectSerialiser::DeserialiseMaterial(const fs::path& resourcePath,
                                                const YAML::Node& resourceData) {
    AE_PROFILE_FUNCTION()

    YAML::Node data;
    try { data = YAML::LoadFile(resourcePath.string()); }
    catch (const std::exception& e) {
        AE_CORE_ERROR("Error deserializing material: {0}", e.what());
        return Resource{};
    }

    Material material;
    material.uuid = UUID::FromStr(resourceData["UUID"].as<std::string>());
    material.name = resourceData["Name"].as<std::string>();

    auto attributes = data["Attributes"];
    if (!attributes) {
        AE_CORE_ERROR("Invalid material!");
        return Resource{};
    }

    for (auto attribute : attributes) {
        if (auto colour = attribute["Colour"]) { material.colour = colour.as<glm::vec4>(); }

        if (auto texture = attribute["Texture"]) {
            if (texture.as<std::string>() != "None")
                material.texture =
                    Texture2D::create(texture.as<std::string>());
        }
    }


    AE_CORE_TRACE("Deserialised Material with name = {0}, UUID = {1}", material.name,
                  material.uuid.toString());

    return static_cast<Resource>(material);
}

void ProjectSerialiser::SerialiseEntity(YAML::Emitter& outYAML, Entity entity) {
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
        }
        else {
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
}; // namespace AnEngine
