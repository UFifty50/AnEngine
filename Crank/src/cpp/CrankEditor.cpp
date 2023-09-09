#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "CrankEditor.hpp"

#include "imgui.h"
#include "imgui_internal.h"

#include "Core/Layer.hpp"
#include "Renderer/FrameBuffer.hpp"
#include "Renderer/RenderCommandQueue.hpp"
#include "Renderer/Renderer2D.hpp"


namespace AnEngine {
    CrankEditor::CrankEditor()
        : Layer("CrankEditor"), cameraController(1280.0f / 720.0f, 75, true, true) {}

    void CrankEditor::onAttach() {
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
            ImGui::Separator();
            ImGui::End();
        });

        dockSpace.addWindow([&]() {
            uint32_t texID = frameBuffer->getColorAttachmentID();

            ImGui::Begin("Viewport");
            ImGui::Image((void*)texID, {1280, 720}, {0, 1}, {1, 0});
            ImGui::End();
        });
    }

    void CrankEditor::onDetach() {}

    void CrankEditor::onUpdate(TimeStep deltaTime) {
        AE_PROFILE_FUNCTION()

        cameraController.setZoom(10.0f);

        {
            AE_PROFILE_SCOPE("Camera")
            cameraController.onUpdate(deltaTime);
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
            frameBuffer->unBind();
        }
    }

    void CrankEditor::onImGuiRender() {
        AE_PROFILE_FUNCTION()

        dockSpace.render();
    }

    void CrankEditor::onEvent(Event& event) {}
}  // namespace AnEngine
