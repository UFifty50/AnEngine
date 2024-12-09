#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <filesystem>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"
#include "Core/UUID.hpp"
#include "File/StreamWriter.hpp"
#include "Project/Resource.hpp"
#include "Project/Resources/Material.hpp"
#include "Project/Resources/Scene/Scene.hpp"
#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"


namespace fs = std::filesystem;

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
        static Project newProject(bool is3D);

        Resource newScene(std::optional<bool> is3D = std::nullopt);

        Directory getParentOfDirectory(const Directory& dir) const {
            return root.findSubDir(dir.parent);
        }

        bool hasActiveScene() const { return activeSceneID != UUID(nullptr); }

        template <typename S>
        std::remove_cvref_t<S>& getActiveScene() {
            Resource& scene = resources.at(activeSceneID);
            if (scene.type == Resource::Type::Scene3D)
                return static_cast<std::remove_cvref_t<
                    S>&>(scene);
            return static_cast<std::remove_cvref_t<S>&>(scene);
        }

        bool isPathInProject(const fs::path& path) {
            for (DirectoryIterator dirEnt = root.begin(); dirEnt != root.end(); ++dirEnt) {
                const auto& dirE = *dirEnt;
                if (dirE.entryType == DirectoryEntry::File) {
                    const File f = *static_cast<const File*>(&dirE);
                    if (equivalent(f.path, path)) return true;
                }
            }
        }

        void loadResourcePath(const fs::path& path) {
            // must load files from within project
        }

        // template <typename S>
        // const S getScene(AnEngine::UUID uuid) const {
        //     Resource scene = resources.at(uuid);
        //     if (scene.type == Resource::Type::Scene3D)
        //         return static_cast<Scene3D>(scene);
        //     else
        //         return static_cast<Scene2D>(scene);
        // }

        // template <typename S>
        // const S getScene(std::string name) const {
        //     for (auto& [_, resource] : resources) {
        //         if (resource.name == name) {
        //             if (resource.type == Resource::Type::Scene3D)
        //                 return static_cast<Scene3D>(resource);
        //             else
        //                 return static_cast<Scene2D>(resource);
        //         }
        //     }
        // }

        Directory& getRootDir() { return root; }

    private:
        std::string name;
        fs::path path;
        bool is3D;

        struct MetaData {
            std::string author;
            std::string version;
            std::string created;
            std::string saved;
            UUID uuid;
        } meta;

        std::unordered_map<UUID, Resource, UUIDHasher> resources;
        UUID activeSceneID = UUID(nullptr);

        Directory root;

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