#include <AnEngine.hpp>
#include <glm/glm.hpp>

#define UNIFORMS(varName, ...) AnEngine::ShaderUniformVector varName = {__VA_ARGS__}


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
    AnEngine::ShaderLibrary shaderLibrary;

    AnEngine::Ref<AnEngine::VertexArray> squareVA;
    AnEngine::Ref<AnEngine::VertexBuffer> squareVB;
    AnEngine::Ref<AnEngine::IndexBuffer> squareIB;

    AnEngine::CameraController cameraController;

    glm::vec4 squareColour = {0.0f, 0.0f, 1.0f, 1.0f};
};
