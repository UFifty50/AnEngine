#include "aepch.hpp"

#include "Scene/Project/Project.hpp"


namespace AnEngine {
    // if material exists in materials uuid map, return it
    // otherwise, load it from the directory structure at `root`
    const Material& Project::loadMaterial(const UUID& uuid) const {
        if (materials.contains(uuid)) return materials.at(uuid);

        for (const auto& [files, directories] : root) {

        }
    }
};  // namespace AnEngine
