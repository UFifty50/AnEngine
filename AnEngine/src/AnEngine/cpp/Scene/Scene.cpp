#include "aepch.hpp"

#include <glm/glm.hpp>

#include "Scene/Scene.hpp"

#include "Renderer/Renderer2D.hpp"
#include "Scene/Components.hpp"
#include "Scene/Entity.hpp"


namespace AnEngine {
    Scene::Scene() {}
    Entity Scene::createEntity(const std::string& name) {
        Entity e = {entityRegistry.create(), this};
        e.addComponent<TransformComponent>();
        auto& tag = e.addComponent<TagComponent>(name);
        tag.Tag = name.empty() ? "Entity" : name;
        return e;
    }

    void Scene::onUpdate(TimeStep deltaTime) {
        entityRegistry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc) {
            if (!nsc.Instance) {
                nsc.instantiateScriptInstance();
                nsc.Instance->entity = Entity{entity, this};

                nsc.onCreate();
            }

            nsc.onUpdate(deltaTime);
        });

        ComponentCamera* mainCamera = nullptr;
        glm::mat4* mainCameraTransform = nullptr;

        auto cameraView = entityRegistry.view<TransformComponent, CameraComponent>();
        for (auto entity : cameraView) {
            const auto& [transform, camera] =
                cameraView.get<TransformComponent, CameraComponent>(entity);
            if (camera.Primary) {
                mainCamera = &camera.Camera;
                mainCameraTransform = &transform.Transform;
                break;
            }
        }

        if (mainCamera) {
            Renderer2D::beginScene(mainCamera->getProjectionMatrix(),
                                   *mainCameraTransform);

            auto spriteView = entityRegistry.group<TransformComponent>(
                entt::get<SpriteRendererComponent>);
            // entityRegistry.view<TransformComponent, SpriteRendererComponent>();

            for (auto entity : spriteView) {
                const auto& [transform, sprite] =
                    spriteView.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::drawQuad(transform, sprite.Colour);
            }

            Renderer2D::endScene();
        } else {
            AE_CORE_WARN("No primary camera entity found!");
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
}  // namespace AnEngine
