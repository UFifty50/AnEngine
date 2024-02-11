#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Scene/Scene3D.hpp"

#include "Core/UUID.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Renderer3D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine {
    Entity& Scene3D::createEntity(const std::string& name) {
        AE_PROFILE_FUNCTION()

        Entity e = {this->entityRegistry.create(), this};
        e.addComponent<IDComponent>();
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        return e;
    }

    Entity& Scene3D::createEntityWithUUID(const std::string& name, UUID id) {
        AE_PROFILE_FUNCTION()

        Entity e = {this->entityRegistry.create(), this};
        e.addComponent<IDComponent>(id);
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        return e;
    }

    void Scene3D::destroyEntity(Entity& entity) { this->entityRegistry.destroy(entity); }

    void Scene3D::onUpdateEditor(TimeStep deltaTime, const Ref<EditorCamera>& camera) {
        AE_PROFILE_FUNCTION()

        Ref<EditorCamera3D> camera3D = std::static_pointer_cast<EditorCamera3D>(camera);
        Renderer3D::beginScene(*camera3D);

        auto objectGroup =
            this->entityRegistry.group<TransformComponent>(entt::get<ObjectRendererComponent>);

        for (auto entity : objectGroup) {
            auto [transform, object] =
                objectGroup.get<TransformComponent, ObjectRendererComponent>(entity);

            Renderer3D::drawObject((glm::mat4)transform, object, (uint32_t)entity);
        }

        Renderer3D::endScene();
    }

    void Scene3D::onUpdateRuntime(TimeStep deltaTime) {
        AE_PROFILE_FUNCTION()

        this->entityRegistry.view<NativeScriptComponent>().each([=](const auto entity,
                                                                    auto& nsc) {
            // TODO: Move to onScenePlay functiom
            if (!nsc.Instance) {
                AE_CORE_ASSERT(nsc.instantiateScriptInstance, "Script {0} not bound!",
                               nsc.Name);
                nsc.instantiateScriptInstance();

                AE_CORE_ASSERT(nsc.Instance,
                               "Script {0} constructor not implemented properly!", nsc.Name);
                nsc.Instance->entity = Entity{entity, this};

                nsc.Instance->onCreate();
            }

            nsc.Instance->onUpdate(deltaTime);
        });

        Scope<Camera> mainCamera = nullptr;
        glm::mat4 mainCameraTransform;

        auto cameraView = this->entityRegistry.view<TransformComponent, CameraComponent>();
        for (auto entity : cameraView) {
            auto [transform, camera] =
                cameraView.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary) {
                mainCamera.reset(&camera.Camera);
                mainCameraTransform = (glm::mat4)transform;
                break;
            }
        }

        if (mainCamera) {
            Renderer3D::beginScene(std::move(mainCamera), mainCameraTransform);

            auto objectGroup = this->entityRegistry.group<TransformComponent>(
                entt::get<ObjectRendererComponent>);

            for (auto entity : objectGroup) {
                auto [transform, object] =
                    objectGroup.get<TransformComponent, ObjectRendererComponent>(entity);

                if ((uint32_t)entity > unsigned(std::numeric_limits<int32_t>::max())) {
                    AE_CORE_ASSERT(false, "Too many entities in scene :(     TODO: fixme");
                }

                Renderer3D::drawObject((glm::mat4)transform, object, (int32_t)entity);
            }

            Renderer3D::endScene();
        }
    }

    void Scene3D::onResize(uint32_t width, uint32_t height) {
        AE_PROFILE_FUNCTION()

        this->viewportWidth = width;
        this->viewportHeight = height;

        auto view = this->entityRegistry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio) {
                cameraComponent.Camera.setViewportSize(width, height);
            }
        }
    }

    Entity Scene3D::getPrimaryCamera() {
        AE_PROFILE_FUNCTION()

        auto view = this->entityRegistry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.Primary) {
                return Entity{entity, this};
            }
        }
        return {};
    }

    void Scene3D::onComponentAdded(Entity& e, Component& component) {
        AE_PROFILE_FUNCTION()

        switch (component.getID()) {
            case CAMERA_COMPONENT_ID: {
                if (this->viewportWidth <= 0 || this->viewportHeight <= 0) return;

                auto& cameraComponent = (CameraComponent&)component;
                cameraComponent.Camera.setViewportSize(this->viewportWidth,
                                                       this->viewportHeight);
                break;
            }

            default:
                break;
        }
    }
}  // namespace AnEngine
