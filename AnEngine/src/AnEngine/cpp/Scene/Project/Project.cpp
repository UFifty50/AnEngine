#include "aepch.hpp"

#include "Project/Project.hpp"

#include "Project/Resources/Scene/Scene2D.hpp"
#include "Project/Resources/Scene/Scene3D.hpp"


namespace AnEngine {
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
};  // namespace AnEngine
