#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Scene/Scene2D.hpp"

#include "Core/UUID.hpp"
#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine {
    Entity& Scene2D::createEntity(const std::string& name) {
        AE_PROFILE_FUNCTION()

        Entity e = {this->entityRegistry.create(), this};
        e.addComponent<IDComponent>();
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        return e;
    }

    Entity& Scene2D::createEntityWithUUID(const std::string& name, UUID id) {
        AE_PROFILE_FUNCTION()

        Entity e = {this->entityRegistry.create(), this};
        e.addComponent<IDComponent>(id);
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        return e;
    }

    void Scene2D::destroyEntity(Entity& entity) { this->entityRegistry.destroy(entity); }

    void Scene2D::onUpdateEditor(TimeStep deltaTime, const Ref<EditorCamera>& camera) {
        AE_PROFILE_FUNCTION()

        AE_CORE_ASSERT(!camera->isPerspective(), "2D EditorCamera must be orthographic!");

        Ref<EditorCamera2D> camera2D = std::static_pointer_cast<EditorCamera2D>(camera);
        Renderer2D::beginScene(*camera2D);

        auto spriteGroup =
            this->entityRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : spriteGroup) {
            auto [transform, sprite] =
                spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::drawSprite((glm::mat4)transform, sprite, (uint32_t)entity);
        }

        Renderer2D::endScene();
    }

    void Scene2D::onUpdateRuntime(TimeStep deltaTime) {
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

        Scope<Camera> mainCamera;
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
            Renderer2D::beginScene(std::move(mainCamera), mainCameraTransform);

            auto spriteGroup = this->entityRegistry.group<TransformComponent>(
                entt::get<SpriteRendererComponent>);

            for (auto entity : spriteGroup) {
                auto [transform, sprite] =
                    spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                if ((uint32_t)entity > unsigned(std::numeric_limits<int32_t>::max())) {
                    AE_CORE_ASSERT(false, "Too many entities in scene :(");  // TODO: fixme
                }

                Renderer2D::drawSprite((glm::mat4)transform, sprite, (int32_t)entity);
            }

            Renderer2D::endScene();
        }
    }

    void Scene2D::onResize(uint32_t width, uint32_t height) {
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

    Entity Scene2D::getPrimaryCamera() {
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

    void Scene2D::onComponentAdded(Entity& e, Component& component) {
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
