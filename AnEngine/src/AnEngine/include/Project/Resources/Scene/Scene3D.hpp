#ifndef SCENE3D_HPP
#define SCENE3D_HPP

#define NOMINMAX
#include <entt/entt.hpp>

#include "Core/UUID.hpp"
#include "Project/Resource.hpp"
#include "Project/Resources/Scene/Scene.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Time/TimeStep.hpp"


namespace AnEngine {
class Scene3D : public Scene {
public:
    ~Scene3D() override = default;

    static Resource Create(const std::string& name = "") {
        return static_cast<Resource>(Scene3D(name));
    }

    void empty() {
        name = "";
        entityRegistry->clear();
    }

    const std::string& getName() const override { return name; }
    void setName(const std::string& newName) override { name = newName; }

    Entity& createEntity(const std::string& name = "") override;
    Entity& createEntityWithUUID(const std::string& name, UUID id) override;
    void destroyEntity(Entity& entity) override;
    void onResize(uint32_t width, uint32_t height) override;

    void onUpdateEditor(TimeStep deltaTime,
                        const Ref<EditorCamera>& camera) override;
    void onUpdateRuntime(TimeStep deltaTime) override;

    Entity getPrimaryCamera();

private:
    Scene3D(const std::string& name = "") : Scene{Type::Scene3D, {}, name} {}

    void onComponentAdded(Entity& e, Component& component) override;
};
} // namespace AnEngine

#endif
