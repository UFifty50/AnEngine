#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include "Core/Core.hpp"
#include "Menus/Filemenu.hpp"
#include "Panels/ContentBrowserPanel.hpp"
#include "Panels/PropertiesPanel.hpp"
#include "Panels/ScenesPanel.hpp"
#include "Panels/StatisticsPanel.hpp"
#include "Panels/ViewportPanel.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine::Crank {
    extern Ref<Scene> g_ActiveScene;

    extern Ref<DockSpace> g_DockSpace;

    extern Ref<ScenesPanel> gPanel_SceneHierarchy;
    extern Ref<PropertiesPanel> gPanel_Properties;
    extern Ref<ViewportPanel> gPanel_Viewport;
    extern Ref<ContentBrowserPanel> gPanel_ContentBrowser;
    extern Ref<StatisticsPanel> gPanel_Statistics;

    extern Ref<FileMenu> gMenu_FileOps;
};  // namespace AnEngine::Crank

#endif
