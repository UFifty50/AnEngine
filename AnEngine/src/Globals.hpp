#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#ifdef AE_CRANKUI
    #include "Dockspace.hpp"
    #include "Menus/Filemenu.hpp"
    #include "Panels/ContentBrowserPanel.hpp"
    #include "Panels/PropertiesPanel.hpp"
    #include "Panels/ScenesPanel.hpp"
    #include "Panels/StatisticsPanel.hpp"
    #include "Panels/ViewportPanel.hpp"
#endif

#include "Core/Core.hpp"
#include "Renderer/Shader.hpp"
#include "Scene/Scene.hpp"


namespace AnEngine {
    extern ShaderLibrary g_ShaderLibrary;

#ifdef AE_CRANKUI
    namespace Crank {
        extern Ref<DockSpace> g_DockSpace;

        extern Ref<FileMenu> gMenu_FileOps;

        extern Ref<ScenesPanel> gPanel_SceneHierarchy;
        extern Ref<PropertiesPanel> gPanel_Properties;
        extern Ref<ViewportPanel> gPanel_Viewport;
        extern Ref<ContentBrowserPanel> gPanel_ContentBrowser;
        extern Ref<StatisticsPanel> gPanel_Statistics;

        extern Ref<Scene> g_ActiveScene;

        extern const fs::path g_BaseAssetsDirectory;
    }  // namespace Crank
#endif
};  // namespace AnEngine

#endif
