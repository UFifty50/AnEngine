#ifndef SANDBOX2D_HPP
#define SANDBOX2D_HPP

#include <AnEngine.hpp>
#include <glm/glm.hpp>

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
    AnEngine::Ref<AnEngine::Texture2D> texture;

    glm::vec4 squareColour = {0.2f, 0.3f, 0.8f, 1.0f};
    glm::vec4 squareColour2 = {0.8f, 0.3f, 0.2f, 1.0f};
};

#endif
