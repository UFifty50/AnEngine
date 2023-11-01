workspace "AnEngine"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    flags { "MultiProcessorCompile" }
    startproject "Crank"
    debugdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Crank"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir['GLFW'] = "%{wks.location}/AnEngine/vendor/GLFW/include"
includeDir['Glad'] = "%{wks.location}/AnEngine/vendor/Glad/include"
includeDir['ImGui'] = "%{wks.location}/AnEngine/vendor/ImGui/"
includeDir['glm'] = "%{wks.location}/AnEngine/vendor/glm/"
includeDir['fmt'] = "%{wks.location}/AnEngine/vendor/fmt/include/"
includeDir['stb'] = "%{wks.location}/AnEngine/vendor/stb/"
includeDir['entt'] = "%{wks.location}/AnEngine/vendor/ENTT/single_include/"
includeDir['yaml_cpp'] = "%{wks.location}/AnEngine/vendor/yaml-cpp/include/"
includeDir['ImGuizmo'] = "%{wks.location}/AnEngine/vendor/ImGuizmo-CE/"

group "Dependencies"
    include "AnEngine/vendor/GLFW"
    include "AnEngine/vendor/Glad"
    include "AnEngine/vendor/ImGui"
    include "AnEngine/vendor/fmt"
    include "AnEngine/vendor/ENTT"
    include "AnEngine/vendor/yaml-cpp"

group ""
project "AnEngine"
    location "AnEngine"
    kind "StaticLib"
    staticruntime "on"
    language "C++"
    cppdialect "C++20"
    
    externalanglebrackets "on"
    externalwarnings "off"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    pchheader "aepch.hpp"
    pchsource "AnEngine/src/aepch.cpp"

    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb/**.cpp",
        "%{prj.name}/vendor/stb/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/ImGuizmo-CE/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo-CE/ImGuizmo.cpp"
    }
    removefiles { "%{prj.name}/src/Platform/**" }
    files {
        "%{prj.name}/src/Platform/OpenGL/**.hpp",
        "%{prj.name}/src/Platform/OpenGL/**.cpp",
    }

    includedirs { 
        "%{prj.name}/src/AnEngine/include/",
        "%{prj.name}/src/Platform/",
        "%{prj.name}/src",
        "%{includeDir.fmt}",
        "%{includeDir.GLFW}",
        "%{includeDir.Glad}",
        "%{includeDir.ImGui}",
        "%{includeDir.glm}",
        "%{includeDir.stb}",
        "%{includeDir.entt}",
        "%{includeDir.yaml_cpp}",
        "%{includeDir.ImGuizmo}"
    }

    externalincludedirs {
        "%{prj.name}/vendor/spdlog/include/"
    }

    defines {
        "YAML_CPP_STATIC_DEFINE"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
        "fmt",
        "yaml-cpp"
    }

    prebuildcommands {
            "{RMDIR} ../bin/" .. outputDir,
    }

    postbuildcommands {
            "{MKDIR} ../bin/" .. outputDir .. "/Crank",
            "{MKDIR} ../bin/" .. outputDir .. "/Crank/builtins",
            "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Crank",
            "{COPYDIR} ../Crank/assets ../bin/" .. outputDir .. "/Crank/assets",
            "{COPYDIR} ../AnEngine/assets/ ../bin/" .. outputDir .. "/Crank/builtins/assets"
    }

    filter "files:AnEngine/vendor/ImGuizmo-CE/**.cpp"
        flags { "NoPCH" }

    filter "system:linux"
        pic "on"
        systemversion "latest"

        files {
            "%{prj.name}/src/Platform/Linux/**.hpp",
            "%{prj.name}/src/Platform/Linux/**.cpp"
        }

        links {
            "Xrandr",
            "Xi",
            "GLEW",
            "GLU",
            "GL",
            "X11",
            "dl",
            "pthread"
        }

        defines { 
            "AE_LINUX",
        }

    filter "system:windows"
        systemversion "latest"

        files {
            "%{prj.name}/src/Platform/Windows/**.hpp",
            "%{prj.name}/src/Platform/Windows/**.cpp"
        }

        links {
            "opengl32.lib",
            "shlwapi.lib"
        }

        defines { 
            "AE_WIN",
            "GLFW_INCLUDE_NONE"
        }

    filter "toolset:msc*"
        buildoptions { 
            "/analyze:external-",
            "/Zc:preprocessor"
        }

        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "toolset:gcc"
        buildoptions {
            "-msse2"
        }

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "AE_RELEASE" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"

project "Crank"
    location "Crank"
    kind "ConsoleApp"
    staticruntime "on"
    language "C++"
    cppdialect "C++20"

    links {
        "AnEngine",
    }

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }

    includedirs { 
        "%{prj.name}/src/include/",
        "AnEngine/src",
        "AnEngine/src/AnEngine/include/",
        "%{includeDir.ImGui}",
        "%{includeDir.entt}",
        "%{includeDir.glm}",
        "%{includeDir.fmt}",
        "%{includeDir.yaml_cpp}",
        "%{includeDir.ImGuizmo}"
    }

    externalincludedirs {
        "AnEngine/vendor/spdlog/include/"
    }

    defines {
        "YAML_CPP_STATIC_DEFINE"
    }

    postbuildcommands {
            "{MKDIR} ../bin/" .. outputDir .. "/Crank/builtins",
            "{COPYDIR} ../Crank/assets ../bin/" .. outputDir .. "/Crank/assets",
            "{COPYDIR} ../AnEngine/assets/ ../bin/" .. outputDir .. "/Crank/builtins/assets"
    }

    filter "system:Linux"
        pic "on"
        systemversion "latest"

        defines { 
            "AE_LINUX",
        }

    filter "system:windows"
        systemversion "latest"

        defines { 
            "AE_WIN",
        }

    filter "toolset:msc*"
        buildoptions {
            "/analyze:external-",
            "/Zc:preprocessor"
        }

        defines {
            "_CRT_SECURE_NO_WARNINGS"
        }

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "AE_RELEASE" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"
