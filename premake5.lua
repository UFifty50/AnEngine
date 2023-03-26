workspace "AnEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }
    startproject "Sandbox"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir['GLFW'] = "AnEngine/vendor/GLFW/include"
includeDir['Glad'] = "AnEngine/vendor/Glad/include"
includeDir['ImGui'] = "AnEngine/vendor/ImGui/"
includeDir['glm'] = "AnEngine/vendor/glm/"

group "Dependencies"
    include "AnEngine/vendor/GLFW"
    include "AnEngine/vendor/Glad"
    include "AnEngine/vendor/ImGui"

group ""
project "AnEngine"
    location "AnEngine"
    kind "SharedLib"
    staticruntime "off"
    language "C++"

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    pchheader "aepch.hpp"
    pchsource "AnEngine/src/aepch.cpp"

    files {
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl"
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
        "%{prj.name}/vendor/spdlog/include/",
        "%{includeDir.GLFW}",
        "%{includeDir.Glad}",
        "%{includeDir.ImGui}",
        "%{includeDir.glm}"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
    }

    filter "system:linux"
        pic "On"
        cppdialect "C++20"
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
            "AE_DLL",
        }

        prebuildcommands {
                "{RMDIR} ../bin/" .. outputDir
        }

        postbuildcommands {
                "{MKDIR} ../bin/" .. outputDir .. "/Sandbox",
                "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Sandbox"
        }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
        buildoptions { "/external:W0" }

        files {
            "%{prj.name}/src/Platform/Windows/**.hpp",
            "%{prj.name}/src/Platform/Windows/**.cpp"
        }

        links {
            "opengl32.lib"
        }

        defines { 
            "AE_WIN",
            "AE_DLL",
            "GLFW_INCLUDE_NONE"
        }

        prebuildcommands {
                "{RMDIR} ../bin/" .. outputDir
        }

        postbuildcommands {
                "{MKDIR} ../bin/" .. outputDir .. "/Sandbox",
                "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Sandbox"
        }

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "AE_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        runtime "Release"
        optimize "On"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    staticruntime "off"
    language "C++"

    links { "AnEngine" }

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }

    includedirs { 
        "%{prj.name}/src/include/",
        "AnEngine/src",
        "AnEngine/src/AnEngine/include/",
        "%{prj.name}/src/Platform/",
        "AnEngine/vendor/spdlog/include/",
        "%{includeDir.glm}"
    }

    filter "system:Linux"
        pic "On"
        cppdialect "C++20"
        systemversion "latest"

        defines { 
            "AE_LINUX",
        }

    filter "system:windows"
        cppdialect "C++20"
        systemversion "latest"
        buildoptions { "/external:W0" }

        defines { 
            "AE_WIN",
        }

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG" }
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines { "AE_RELEASE" }
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        runtime "Release"
        optimize "On"
