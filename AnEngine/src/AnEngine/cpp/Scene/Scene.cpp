#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Scene/Scene.hpp"

#include "Renderer/Camera/EditorCamera.hpp"
#include "Renderer/Renderer2D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine {
    Entity Scene::createEntity(const std::string& name) {
        Entity e = {entityRegistry.create(), this};
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name.empty() ? "Entity" : name);
        return e;
    }

    void Scene::destroyEntity(Entity entity) { entityRegistry.destroy(entity); }

    void Scene::onUpdateEditor(TimeStep deltaTime, const Ref<EditorCamera>& camera) {
        Renderer2D::beginScene(*camera);

        auto spriteGroup =
            entityRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

        for (auto entity : spriteGroup) {
            auto [transform, sprite] =
                spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::drawQuad((glm::mat4)transform, sprite.Colour, (uint32_t)entity);
        }

        Renderer2D::endScene();
    }

    void Scene::onUpdateRuntime(TimeStep deltaTime) {
        entityRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
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

        auto cameraView = entityRegistry.view<TransformComponent, CameraComponent>();
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
            Renderer2D::beginScene(mainCamera->getProjectionMatrix(), mainCameraTransform);

            auto spriteGroup =
                entityRegistry.group<TransformComponent>(entt::get<SpriteRendererComponent>);

            for (auto entity : spriteGroup) {
                auto [transform, sprite] =
                    spriteGroup.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawQuad((glm::mat4)transform, sprite.Colour, (uint32_t)entity);
            }

            Renderer2D::endScene();
        }
    }

    void Scene::onResize(uint32_t width, uint32_t height) {
        viewportWidth = width;
        viewportHeight = height;

        auto view = entityRegistry.view<CameraComponent>();
        for (auto entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);
            if (!cameraComponent.FixedAspectRatio) {
                cameraComponent.Camera.setViewportSize(width, height);
            }
        }
    }

    Entity Scene::getPrimaryCamera() {
        auto view = entityRegistry.view<CameraComponent>();
        for (auto entity : view) {
            const auto& cameraComponent = view.get<CameraComponent>(entity);
            if (cameraComponent.Primary) {
                return Entity{entity, this};
            }
        }
        return {};
    }

    void Scene::onComponentAdded(Entity e, Component& component) {
        switch (component.getID()) {
            case CAMERA_COMPONENT_ID: {
                auto& cameraComponent = (CameraComponent&)component;
                cameraComponent.Camera.setViewportSize(viewportWidth, viewportHeight);
                break;
            }

            default:
                break;
        }
    }
}  // namespace AnEngine
