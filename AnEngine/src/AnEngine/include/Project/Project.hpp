#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <filesystem>
#include <optional>
#include <queue>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "Core/UUID.hpp"
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
        AnEngine::UUID uuid;
        std::string name;


        bool operator==(const FileSystemItem& other) const { return uuid == other.uuid; }
    };
    struct FileSystemItemHasher {
        size_t operator()(const AnEngine::FileSystemItem file) const {
            return UUIDHasher{}(file.uuid);
        }
    };

    struct File : public FileSystemItem {
        FileType type;
        fs::path path;

        File(AnEngine::UUID uuid, std::string name, FileType type, fs::path path)
            : FileSystemItem{DirectoryEntry::File, uuid, name}, type(type), path(path) {}
    };

    struct DirectoryIterator;
    struct Directory : public FileSystemItem {
        std::unordered_set<File, FileSystemItemHasher> files;
        std::unordered_set<Directory, FileSystemItemHasher> directories;

        Directory() : FileSystemItem{DirectoryEntry::Directory} {}
        Directory(AnEngine::UUID uuid, std::string name,
                  std::unordered_set<File, FileSystemItemHasher> files,
                  std::unordered_set<Directory, FileSystemItemHasher> directories)
            : FileSystemItem{DirectoryEntry::Directory, uuid, name},
              files(files),
              directories(directories) {}


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

        bool hasActiveScene() const { return activeSceneID != AnEngine::UUID(nullptr); }

        template <typename S>
        std::remove_cvref_t<S>& getActiveScene() {
            Resource& scene = resources.at(activeSceneID);
            if (scene.type == Resource::Type::Scene3D)
                return static_cast<std::remove_cvref_t<S>&>(scene);
            else
                return static_cast<std::remove_cvref_t<S>&>(scene);
        }

        bool isPathInProject(const fs::path& path) {
            for (DirectoryIterator dirEnt = root.begin(); dirEnt != root.end(); dirEnt++) {
                const auto& dirE = *dirEnt;
                if (dirE.entryType == DirectoryEntry::File) {
                    const File f = *static_cast<const File*>(&dirE);
                    if (fs::equivalent(f.path, path)) return true;
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


    private:
        std::string name;
        fs::path path;
        bool is3D;

        struct MetaData {
            std::string author;
            std::string version;
            std::string created;
            std::string saved;
            AnEngine::UUID uuid;
        } meta;

        std::unordered_map<AnEngine::UUID, Resource, UUIDHasher> resources;
        AnEngine::UUID activeSceneID = AnEngine::UUID(nullptr);

        Directory root;

        friend class ProjectSerialiser;
        friend struct YAML::convert<Project::MetaData>;
    };

}  // namespace AnEngine


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
