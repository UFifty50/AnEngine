#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CrankEditor.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Input.hpp"
#include "Core/Layer.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Renderer2D.hpp"


namespace AnEngine {
    CrankEditor::CrankEditor()
        : Layer("CrankEditor"),
          cameraController(1280.0f / 720.0f, 75, true, true),
          viewportSize(0.0f),
          viewportPos(0.0f),
          mousePosInViewport(0.0f),
          sheet("assets/textures/RPGpack_sheet_2X.png", 128, 128) {}

    void CrankEditor::onAttach() {
        sprite1 = sheet.getSprite({4, 9});

        FrameBufferSpec spec = {1280, 720};
        frameBuffer = FrameBuffer::create(spec);

        dockSpace.addWindow([&]() {
            ImGui::Begin("Statistics");
            ImGui::Text("Renderer2D Stats:");
            ImGui::Text("Draw Calls: %d", Renderer2D::getStats().draws);
            ImGui::Text("Quads: %d", Renderer2D::getStats().quadCount);
            ImGui::Text("Vertices: %d", Renderer2D::getStats().getTotalVertexCount());
            ImGui::Text("Indices: %d", Renderer2D::getStats().getTotalIndexCount());
            ImGui::End();
        });

        dockSpace.addWindow([&]() {
            ImGui::Begin("Settings");
            ImGui::Text("Camera Position: %f, %f, %f",
                        cameraController.getCamera()->getPosition().x,
                        cameraController.getCamera()->getPosition().y,
                        cameraController.getCamera()->getPosition().z);
            ImGui::DragInt3("Camera Position",
                            (int*)&cameraController.getCamera()->getPosition());
            ImGui::ColorButton("Background Colour", {0.1f, 0.1f, 0.1f, 1.0f});
            ImGui::End();
        });

        dockSpace.addWindow([&]() {
            uint32_t texID = frameBuffer->getColorAttachmentID();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
            ImGui::Begin("Viewport");

            viewportFocused = ImGui::IsWindowFocused();
            viewportHovered = ImGui::IsWindowHovered();
            Application::getImGuiLayer()->shouldAllowEvents(viewportFocused &&
                                                            viewportHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
            ImVec2 mousePos = ImGui::GetMousePos();
            ImVec2 windowPos = ImGui::GetWindowPos();
            ImVec2 rootViewportPos = ImGui::GetMainViewport()->Pos;

            ImVec2 vpPos =
                ImVec2(windowPos.x - rootViewportPos.x, windowPos.y - rootViewportPos.y);
            ImVec2 mPosInVp = ImVec2(mousePos.x - windowPos.x, mousePos.y - windowPos.y);

            viewportSize = glm::vec2{viewportPanelSize.x, viewportPanelSize.y};
            mousePosInViewport = glm::vec2{mPosInVp.x, mPosInVp.y};
            viewportPos = glm::vec2{vpPos.x, vpPos.y};

            ImGui::Image((void*)texID, ImVec2{viewportSize.x, viewportSize.y}, {0, 1},
                         {1, 0});
            ImGui::End();
            ImGui::PopStyleVar();
        });
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        AE_PROFILE_FUNCTION()

        //   cameraController.setZoom(10.0f);
        if (viewportFocused) cameraController.onUpdate(deltaTime);

        if (viewportSize.x > 0.0f && viewportSize.y > 0.0f) {
            frameBuffer->resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
            cameraController.onResize(viewportSize.x, viewportSize.y);
        }

        Renderer2D::resetStats();
        Renderer2D::getStats().lastFrameTime = deltaTime.getMilliseconds();
        AE_CORE_INFO("FrameTime: {0}     FPS: {1}", deltaTime.getMilliseconds(),
                     1.0f / deltaTime);

        {
            AE_PROFILE_SCOPE("Render Commands")
            frameBuffer->bind();
            RenderCommandQueue::clearColour({0.1f, 0.1f, 0.1f, 1});
            RenderCommandQueue::clear();
        }

        {
            AE_PROFILE_SCOPE("Scene")

            auto camBounds = cameraController.getOrthographicCamera()->getBounds();
            glm::vec3 pos = cameraController.getCamera()->getPosition();

            float x = (mousePosInViewport.x / viewportSize.x) * camBounds.getWidth() -
                      camBounds.getWidth() * 0.5f;
            float y = camBounds.getHeight() * 0.5f -
                      (mousePosInViewport.y / viewportSize.y) * camBounds.getHeight();

            Renderer2D::beginScene(cameraController.getCamera());

            sprite1.render({x, y, 0.0f}, 0.0f);

            Renderer2D::endScene();

            frameBuffer->unBind();
        }
    }

    void CrankEditor::onImGuiRender() {
        AE_PROFILE_FUNCTION()

        dockSpace.render();
    }

    void CrankEditor::onEvent(Event& event) { cameraController.onEvent(event); }
}  // namespace AnEngine
