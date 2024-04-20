#include "aepch.hpp"

#include "Project/Project.hpp"

#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"


namespace AnEngine {
    //////////////////////////////////////////
    // Project
    //////////////////////////////////////////
    Project Project::newProject(bool is3D) {
        Project project = Project();
        project.is3D = is3D;
        return project;
    }

    // if `isScene3D` is not provided, use the project's default
    Resource Project::newScene(std::optional<bool> isScene3D) {
        Resource sceneRes;
        if (isScene3D.has_value()) {
            if (*isScene3D) {
                sceneRes = Scene3D::create("");
                resources[sceneRes.uuid] = sceneRes;
            }
        } else {
            sceneRes = Scene2D::create("");
            resources[sceneRes.uuid] = sceneRes;
        }

        activeSceneID = sceneRes.uuid;
        return sceneRes;
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
        for (const auto& entry : root.directories)
            dirQueue.push(const_cast<Directory*>(&entry));
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
                dirQueue.push(const_cast<Directory*>(&entry));
            for (const auto& entry : nextDir->files) fileQueue.push(entry);
        } else if (!fileQueue.empty()) {
            fileQueue.pop();
        } else {
            isEnd = true;
        }
        return *this;
    }

    FileSystemItem& DirectoryIterator::operator*() {
        if (!dirQueue.empty())
            return *dirQueue.front();
        else if (!fileQueue.empty())
            return fileQueue.front();
        else
            throw std::runtime_error("Iterator is at end");
    }
};  // namespace AnEngine
