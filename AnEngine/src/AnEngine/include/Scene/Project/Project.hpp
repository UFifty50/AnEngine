#ifndef PROJECT_HPP
#define PROJECT_HPP

#include <filesystem>
#include <string>
#include <unordered_map>

#include "Core/UUID.hpp"
#include "Texture/Material.hpp"


namespace fs = std::filesystem;

namespace AnEngine {
    enum class FileType { Material, Model, Texture, Shader, Scene, Other };

    struct File {
        FileType type;
        std::string name;
        fs::path path;
    };

    struct Directory {
        std::unordered_map<std::string, File> files;
        std::unordered_map<std::string, Directory> directories;
    };

    struct Meta {
        FileType type;
        std::string filename;
        fs::path path;

        static Meta fromYAML(const fs::path& path);
    };

    class Project {
    public:
        Project() : path(nullptr) {}

        const Material& loadMaterial(const UUID& uuid) const;
        Material& tweakMaterial(const UUID& uuid);

    private:
        struct MetaData {
            std::string author;
            std::string version;
            std::string date;
        };

        std::string name{"New Project"};
        MetaData meta;
        fs::path path;
        std::unordered_map<UUID, Material> materials;

        Directory root;

        friend class ProjectSerialiser;
    };
}  // namespace AnEngine


// example YAML project file
// name: "Crank"
// meta:
//   - author: "AnEngine"
//   - version: "0.0.1"
//   - date: "2021-10-01"

#endif
