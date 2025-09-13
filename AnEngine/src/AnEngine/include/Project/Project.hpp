#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <filesystem>
#include <optional>
#include <queue>
#include <ranges>
#include <string>
#include <unordered_map>

#include "Core/UUID.hpp"
#include "File/StreamWriter.hpp"
#include "Project/Resource.hpp"
#include "Project/Resources/Material.hpp"
#include "Project/Resources/Scene/Scene.hpp"
#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"
#include "yaml-cpp/yaml.h"


namespace fs = std::filesystem;
namespace ranges = std::ranges;

namespace AnEngine {
enum class DirectoryEntry { Other, File, Directory };

enum class FileType { Other, Material, Model, Texture, Shader, Scene };


struct FileSystemItem {
    DirectoryEntry entryType;
    UUID uuid;
    std::string name;

    bool operator==(const FileSystemItem& other) const { return uuid == other.uuid; }

    bool isFile() const { return entryType == DirectoryEntry::File; }
    bool isDirectory() const { return entryType == DirectoryEntry::Directory; }
};

struct FileSystemItemHasher {
    size_t operator()(const FileSystemItem file) const { return UUIDHasher{}(file.uuid); }
};

struct File : FileSystemItem {
    FileType type;
    fs::path path;
    UUID parent;

    File(const std::string& name, const UUID& parentUUID)
        : FileSystemItem{DirectoryEntry::File, {}, name}, parent(parentUUID) {}

    File(UUID uuid, std::string name, FileType type, fs::path path)
        : FileSystemItem{DirectoryEntry::File, uuid, name}, type(type), path(path) {}
};

struct DirectoryIterator;

struct Directory : FileSystemItem {
    std::unordered_set<File, FileSystemItemHasher> files;
    std::unordered_set<Directory, FileSystemItemHasher> directories;
    UUID parent;

    Directory()
        : FileSystemItem{DirectoryEntry::Directory}, parent(UUID(nullptr)) {}

    Directory(const std::string& name, const UUID& parentUUID)
        : FileSystemItem{DirectoryEntry::Directory, {}, name}, parent(parentUUID) {}

    Directory(UUID uuid, std::string name, UUID parent,
              std::unordered_set<File, FileSystemItemHasher> files,
              std::unordered_set<Directory, FileSystemItemHasher> directories)
        : FileSystemItem{DirectoryEntry::Directory, uuid, name},
          files(files),
          directories(directories),
          parent(parent) {}


    void addDirectory(const std::string& dirName) {
        directories.insert(Directory(dirName, this->uuid));
    }

    Directory findSubDir(UUID uuid) const {
        if (this->uuid == uuid) return *this;
        for (const Directory& dir : directories) {
            Directory subDir = dir.findSubDir(uuid);
            if (subDir.uuid != UUID(nullptr)) return subDir;
        }
        return Directory();
    }

    Directory findContainingDir(UUID uuid) const {
        for (const Directory& dir : directories) {
            if (dir.uuid == uuid) return *this;
            Directory containingDir = dir.findContainingDir(uuid);
            if (containingDir.uuid != UUID(nullptr)) return containingDir;
        }
        return Directory();
    }

    DirectoryIterator begin() const;
    DirectoryIterator end() const;
};


struct DirectoryIterator {
    using iterator_category = std::forward_iterator_tag;
    using value_type = FileSystemItem;
    using difference_type = std::ptrdiff_t;
    using pointer = value_type*;
    using reference = value_type&;


    DirectoryIterator(const Directory& root);

    bool operator==(const DirectoryIterator& other) const;
    DirectoryIterator& operator++(int);
    DirectoryIterator& operator++() { return operator++(0); }
    FileSystemItem& operator*();

private:
    std::queue<Directory*> dirQueue;
    std::queue<File> fileQueue;
    bool isEnd;
};


class Project {
public:
    Project() = default;
    static Project NewProject(bool is3D);

    Resource newScene(std::optional<bool> isScene3D = std::nullopt);

    //Directory getParentOfDirectory(const Directory& dir) const {
    //    return root.findSubDir(dir.parent);
    //}

    [[nodiscard]] bool hasActiveScene() const;

    Resource& getActiveScene() { return resources.at(activeSceneID); }

    [[nodiscard]] bool isPathInProject(const fs::path& path) const;

    //for (DirectoryIterator dirEnt = root.begin(); dirEnt != root.end(); ++dirEnt) {
    //    const auto& dirE = *dirEnt;
    //    if (dirE.entryType == DirectoryEntry::File) {
    //        const File f = *static_cast<const File*>(&dirE);
    //        if (equivalent(f.path, path)) return true;
    //    }
    //}


    fs::path getProjectRootPath() const { return rootPathAbs; }

    Resource getResource(const UUID& uuid) const {
        if (!resources.contains(uuid)) {
            std::string uuidStr = uuid.toString();
            AE_CORE_ASSERT(false, "No resource found for UUID \"{}\".", uuidStr)
            return Resource();
        }
        return resources.at(uuid);
    }

    fs::path getResourcePath(const UUID& uuid) const {
        if (!resourceRelPaths.contains(uuid)) {
            std::string uuidStr = uuid.toString();
            AE_CORE_ASSERT(false, "No resource path found for UUID \"{}\".", uuidStr)
            return fs::path();
        }
        return resourceRelPaths.at(uuid);
    }

    void addResource(const Resource& resource, const fs::path& path) {
        resources[resource.uuid] = resource;
        resourceRelPaths[resource.uuid] = path;
    }

    template <typename S>
    S getScene(const UUID& uuid) const {
        if (!resources.contains(uuid)) {
            AE_CORE_ASSERT(false, "No resource found for UUID \"{}\".", uuid);
            return S();
        }

        Resource scene = resources.at(uuid);
        if (scene.type == Resource::Type::Scene3D) return *reinterpret_cast<const Scene2D*>(&scene);

        if (scene.type == Resource::Type::Scene2D) return *reinterpret_cast<const Scene2D*>(&scene);

        AE_CORE_ASSERT(false, "Resource \"{}\" is not a scene.", uuid);
        return S();
    }

    template <typename S>
    S getScene(const std::string& searchName) const {
        for (const auto& resource : resources | std::views::values) {
            if (resource.name == searchName) {
                if (resource.type == Resource::Type::Scene3D)
                    return *reinterpret_cast<const Scene2D
                        *>(&resource);

                if (resource.type == Resource::Type::Scene2D)
                    return *reinterpret_cast<const Scene2D
                        *>(&resource);

                AE_CORE_ASSERT(false, "Resource \"{}\" is not a scene.", searchName);
                return S();
            }
        }

        AE_CORE_ASSERT(false, "No resource found for name \"{}\".", searchName);
        return S();
    }

    //   Directory& getRootDir() { return root; }

private:
    bool is3D;

    struct MetaData {
        std::string author;
        std::string version;
        std::string created;
        std::string saved;
        UUID uuid;
    } meta;

    std::unordered_map<UUID, Resource, UUIDHasher> resources;
    std::unordered_map<UUID, fs::path, UUIDHasher> resourceRelPaths;

    std::string name;
    fs::path rootPathAbs;
    UUID activeSceneID = UUID(nullptr);

    //TODO: add back as asset manager
    // Directory root;

    friend class ProjectSerialiser;
    friend struct YAML::convert<MetaData>;
};
} // namespace AnEngine


// example YAML project file
// name: "Crank"
// meta:
//   author: "AnEngine"
//   version: "0.0.1"
//   created: "2021-10-01"
//   saved: "2021-10-01"
//   uuid: "12345678-90abcdef"
// root:
//   files:
//     12345678-90abcdef:
//       path: "material.yml"
//       name: "material.yml"
//       type: "Material"
//
//   directories:
//     12345678-90abcdef:
//       name: "models"
//       files:
//         12345678-90abcdef:
//           path: "models/model.obj"
//           name: "model.obj"
//           type: "Model"
//
//     abcdef12-34567890:
//       name: "textures"
//       files:
//         12345678-90abcdef:
//           path: "textures/texture.png"
//           name: "texture.png"
//           type: "Texture"
//       directories:
//         abc12345-67890def:
//           name: "normal"
//           files:
//             12345678-90abcdef:
//               path: "textures/normal/normal.png"
//               name: "normal.png"
//               type: "Texture"

#endif
