#include "Panels/ScenesPanel.hpp"

#include "imgui.h"

namespace AnEngine::Crank {
    ScenesPanel::ScenesPanel(std::string name) : name(name) {}

    void ScenesPanel::begin() { ImGui::Begin(name.c_str()); }

    void ScenesPanel::render() { ImGui::Text("Scene Hierarchy"); }

    void ScenesPanel::end() { ImGui::End(); }
}  // namespace AnEngine::Crank
