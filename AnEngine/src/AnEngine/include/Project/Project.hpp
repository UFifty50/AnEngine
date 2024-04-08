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
    };

    struct File : public FileSystemItem {
        FileType type;
        fs::path path;

        File(AnEngine::UUID uuid, std::string name, FileType type, fs::path path)
            : FileSystemItem{DirectoryEntry::File, uuid, name}, type(type), path(path) {}
    };

    struct Directory : public FileSystemItem {
        std::unordered_set<File> files;
        std::unordered_set<Directory> directories;

        Directory() : FileSystemItem{DirectoryEntry::Directory} {}
        Directory(AnEngine::UUID uuid, std::string name, std::unordered_set<File> files,
                  std::unordered_set<Directory> directories)
            : FileSystemItem{DirectoryEntry::Directory, uuid, name},
              files(files),
              directories(directories) {}

        class Iterator {
        public:
            Iterator(const Directory& root) : isEnd(false) {
                for (const auto& entry : root.directories) dirQueue.push(entry);
                for (const auto& entry : root.files) fileQueue.push(entry);
            }

            bool hasNext() const { return !(dirQueue.empty() && fileQueue.empty()); }

            bool operator==(const Iterator& other) {
                return isEnd == other.isEnd && dirQueue == other.dirQueue &&
                       fileQueue == other.fileQueue;
            }

            Iterator& operator++() {
                if (!dirQueue.empty()) {
                    auto nextDir = dirQueue.front();
                    dirQueue.pop();
                    for (const auto& entry : nextDir.directories) dirQueue.push(entry);
                    for (const auto& entry : nextDir.files) fileQueue.push(entry);
                } else if (!fileQueue.empty()) {
                    fileQueue.pop();
                } else {
                    isEnd = true;
                }
                return *this;
            }

            FileSystemItem& operator*() {
                if (!dirQueue.empty())
                    return dirQueue.front();
                else
                    return fileQueue.front();
            }

        private:
            std::queue<Directory> dirQueue;
            std::queue<File> fileQueue;
            bool isEnd;
        };

        Iterator begin() const { return Iterator(*this); }
        Iterator end() const { return Iterator({}); }
    };

    class Project {
    public:
        static Project newProject(bool is3D);

        Resource newScene(std::optional<bool> is3D = std::nullopt);

        template <typename S>
        std::remove_cvref_t<S>& getActiveScene() {
            Resource& scene = resources.at(activeSceneID);
            if (scene.type == Resource::Type::Scene3D)
                return static_cast<std::remove_cvref_t<S>&>(scene);
            else
                return static_cast<std::remove_cvref_t<S>&>(scene);
        }

        bool isPathInProject(const fs::path& path) {
            for (auto dirEnt : root) {
                if (dirEnt.entryType == DirectoryEntry::File) {
                    File f = *static_cast<File*>(&dirEnt);
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
        Project() = default;

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

        std::unordered_map<AnEngine::UUID, Resource> resources;
        AnEngine::UUID activeSceneID;

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
