#include "aepch.hpp"

#include "Project/Project.hpp"

#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"


namespace AnEngine {
//////////////////////////////////////////
// Project
//////////////////////////////////////////
Project Project::NewProject(bool is3D) {
    auto project = Project();
    project.is3D = is3D;
    return project;
}

// if `isScene3D` is not provided, use the project's default
Resource Project::newScene(std::optional<bool> isScene3D) {
    Resource sceneRes;
    sceneRes.type = is3D ? Resource::Type::Scene3D : Resource::Type::Scene2D;

    if (isScene3D.has_value()) {
        if (*isScene3D) {
            sceneRes = Scene3D::Create("");
            resources[sceneRes.uuid] = sceneRes;
        }
        else {
            sceneRes = Scene2D::Create("");
            resources[sceneRes.uuid] = sceneRes;
        }
    }

    activeSceneID = sceneRes.uuid;
    return sceneRes;
}

[[nodiscard]] bool Project::hasActiveScene() const { return activeSceneID != UUID(nullptr); }

[[nodiscard]] bool Project::isPathInProject(const fs::path& path) const {
    if (!exists(path)) {
        std::string pathStr = path.string();
        AE_CORE_ASSERT(false, "Path \"{}\" does not exist.", pathStr)
        return false;
    }

    fs::path pathAbs = canonical(path);
    if (rootPathAbs == pathAbs) return true;

    // Check if pathAbs starts with rootPathAbs
    return ranges::mismatch(rootPathAbs, pathAbs).in1 == rootPathAbs.end();
}


//////////////////////////////////////////
// Directory
//////////////////////////////////////////
DirectoryIterator Directory::begin() const { return DirectoryIterator(*this); }
DirectoryIterator Directory::end() const { return DirectoryIterator({}); }


//////////////////////////////////////////
// DirectoryIterator
//////////////////////////////////////////
DirectoryIterator::DirectoryIterator(const Directory& root) : isEnd(false) {
    for (const auto& entry : root.directories) dirQueue.push(const_cast<Directory*>(&entry));
    for (const auto& entry : root.files) fileQueue.push(entry);
}

bool DirectoryIterator::operator==(const DirectoryIterator& other) const {
    return isEnd == other.isEnd && dirQueue == other.dirQueue &&
        fileQueue == other.fileQueue;
}

DirectoryIterator& DirectoryIterator::operator++(int) {
    if (!dirQueue.empty()) {
        auto nextDir = dirQueue.front();
        dirQueue.pop();
        for (const auto& entry : nextDir->directories)
            dirQueue.
                push(const_cast<Directory*>(&entry));
        for (const auto& entry : nextDir->files) fileQueue.push(entry);
    }
    else if (!fileQueue.empty()) { fileQueue.pop(); }
    else { isEnd = true; }
    return *this;
}

FileSystemItem& DirectoryIterator::operator*() {
    if (!dirQueue.empty()) return *dirQueue.front();
    if (!fileQueue.empty()) return fileQueue.front();
    throw std::runtime_error("Iterator is at end");
}
}; // namespace AnEngine
