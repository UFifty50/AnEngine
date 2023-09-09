#ifndef SANDBOX2D_HPP
#define SANDBOX2D_HPP

#include <AnEngine.hpp>
#include <glm/glm.hpp>

#include "imgui.h"
#include "imgui_internal.h"

//#define ATTRIBUTE_ARRAY(varName, ...) \
//    AnEngine::ShaderUniformVector varName = {__VA_ARGS__}

#define ATTRIBUTE_ARRAY(...) \
    AnEngine::ShaderUniformVector { __VA_ARGS__ }


class SandBox2D : public AnEngine::Layer {
public:
    SandBox2D();
    virtual ~SandBox2D() = default;

    virtual void onAttach() override;
    virtual void onDetach() override;

    virtual void onUpdate(AnEngine::TimeStep deltaTime) override;
    virtual void onImGuiRender() override;
    virtual void onEvent(AnEngine::Event& event) override;

private:
    AnEngine::CameraController cameraController;
    AnEngine::SpriteSheet tiles;
    std::unordered_map<char, AnEngine::Sprite> tileMap;
    AnEngine::Sprite player;
    AnEngine::Sprite invalidTile;

    AnEngine::Ref<AnEngine::FrameBuffer> frameBuffer;

    AnEngine::ParticleSpawner particleSpawner;

    float playerX = 0.0f;
    float playerY = 0.0f;

    float tilingFactor = 1.0f;
    glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f};

    float sizeVariation = 0.0f;
    float spawnRate = 10.0f;

    bool toggle = false;
};

/*
class TiledWorld2D {
public:
    TiledWorld2D(const std::vector<std::vector<AnEngine::Sprite>>& tiles);

    void render();
};
*/

#endif
