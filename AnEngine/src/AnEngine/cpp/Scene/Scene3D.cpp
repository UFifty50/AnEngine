#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Scene/Scene3D.hpp"

#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Renderer.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine {
    Entity& Scene3D::createEntity(const std::string& name) {
        Entity e = {this->entityRegistry.create(), this};
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        return e;
    }

    void Scene3D::destroyEntity(Entity& entity) { this->entityRegistry.destroy(entity); }

    void Scene3D::onUpdateEditor(TimeStep deltaTime, const Ref<EditorCamera>& camera) {
        //    Renderer::beginScene(*camera);

        auto spriteGroup =
            this->entityRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : spriteGroup) {
            auto [transform, sprite] =
                spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

            //      Renderer::drawObject((glm::mat4)transform, sprite, (uint32_t)entity);
        }

        Renderer::endScene();
    }

    void Scene3D::onUpdateRuntime(TimeStep deltaTime) {
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

        Camera* mainCamera = nullptr;
        glm::mat4 mainCameraTransform;

        auto cameraView = this->entityRegistry.view<TransformComponent, CameraComponent>();
        for (auto entity : cameraView) {
            auto [transform, camera] =
                cameraView.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary) {
                mainCamera = &camera.Camera;
                mainCameraTransform = (glm::mat4)transform;
                break;
            }
        }

        if (mainCamera) {
            //     Renderer::beginScene(mainCamera->getProjectionMatrix(),
            //     mainCameraTransform);

            auto spriteGroup = this->entityRegistry.group<TransformComponent>(
                entt::get<SpriteRendererComponent>);

            for (auto entity : spriteGroup) {
                auto [transform, sprite] =
                    spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                if ((uint32_t)entity > std::numeric_limits<int32_t>::max()) {
                    AE_CORE_ASSERT(false, "Too many entities in scene :(     TODO: fixme");
                }

                //        Renderer::drawObject((glm::mat4)transform, sprite, (int32_t)entity);
            }

            Renderer::endScene();
        }
    }

    void Scene3D::onResize(uint32_t width, uint32_t height) {
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
