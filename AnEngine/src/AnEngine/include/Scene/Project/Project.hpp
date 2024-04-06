#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <filesystem>
#include <queue>
#include <string>
#include <unordered_map>

#include "yaml-cpp/yaml.h"

#include "Core/UUID.hpp"
#include "Texture/Material.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
    enum class DirectoryEntry { Other, File, Directory };
    enum class FileType { Other, Material, Model, Texture, Shader, Scene };

    struct FileSystemItem {
        DirectoryEntry type;
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
        Project() = default;

    private:
        std::string name;
        fs::path path;

        struct MetaData {
            std::string author;
            std::string version;
            std::string created;
            std::string saved;
            AnEngine::UUID uuid;
        } meta;

        std::unordered_map<UUID, Material> materials;
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
