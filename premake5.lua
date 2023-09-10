workspace "AnEngine"
    architecture "x86_64"
    configurations { "Debug", "Release", "Dist" }
    flags { "MultiProcessorCompile" }
    startproject "Crank"
    debugdir "bin/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}/Crank"

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir['GLFW'] = "AnEngine/vendor/GLFW/include"
includeDir['Glad'] = "AnEngine/vendor/Glad/include"
includeDir['ImGui'] = "AnEngine/vendor/ImGui/"
includeDir['glm'] = "AnEngine/vendor/glm/"
includeDir['fmt'] = "AnEngine/vendor/fmt/include/"
includeDir['stb'] = "AnEngine/vendor/stb/"
includeDir['entt'] = "AnEngine/vendor/ENTT/single_include/"

group "Dependencies"
    include "AnEngine/vendor/GLFW"
    include "AnEngine/vendor/Glad"
    include "AnEngine/vendor/ImGui"
    include "AnEngine/vendor/fmt"
    include "AnEngine/vendor/ENTT"

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
        "%{includeDir.GLFW}",
        "%{includeDir.Glad}",
        "%{includeDir.entt}",
        "%{includeDir.ImGui}",
        "%{includeDir.glm}",
        "%{includeDir.fmt}",
        "%{includeDir.stb}"
    }

    externalincludedirs {
        "%{prj.name}/vendor/spdlog/include/"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
        "fmt",
    }

    prebuildcommands {
            "{RMDIR} ../bin/" .. outputDir,
    }

    postbuildcommands {
            "{MKDIR} ../bin/" .. outputDir .. "/Sandbox",
            "{MKDIR} ../bin/" .. outputDir .. "/Crank",
            "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Sandbox",
            "{COPYFILE} %{cfg.buildtarget.relpath} ../bin/" .. outputDir .. "/Crank",
            "{COPYDIR} ../Sandbox/assets ../bin/" .. outputDir .. "/Sandbox/assets",
            "{COPYDIR} ../Crank/assets ../bin/" .. outputDir .. "/Crank/assets",
            "{COPYDIR} ../AnEngine/assets/ ../bin/" .. outputDir .. "/Sandbox/builtins/assets",
            "{COPYDIR} ../AnEngine/assets/ ../bin/" .. outputDir .. "/Crank/builtins/assets"
    }

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
            "AE_DLL",
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
            "AE_DLL",
            "GLFW_INCLUDE_NONE"
        }

    filter "toolset:msc*"
        buildoptions { 
            "/analyze:external-",
            "/Zc:preprocessor"
        }

    filter "toolset:gcc"
        buildoptions {
            "-msse2"
        }

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG", "AE_PROFILING" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "AE_RELEASE", "AE_RELEASE_PROFILING" }
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
        "GLFW",
        "Glad",
        "ImGui",
        "fmt"
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
        "%{includeDir.fmt}"
    }

    externalincludedirs {
        "AnEngine/vendor/spdlog/include/"
    }

    postbuildcommands {
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

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG", "AE_PROFILING" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "AE_RELEASE", "AE_RELEASE_PROFILING" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    staticruntime "on"
    language "C++"
    cppdialect "C++20"

    links {
        "AnEngine",
        "GLFW",
        "Glad",
        "ImGui",
        "fmt"
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
        "%{includeDir.fmt}"
    }

    externalincludedirs {
        "AnEngine/vendor/spdlog/include/"
    }

    postbuildcommands {
            "{COPYDIR} ../Sandbox/assets ../bin/" .. outputDir .. "/Sandbox/assets",
            "{COPYDIR} ../AnEngine/assets/ ../bin/" .. outputDir .. "/Sandbox/builtins/assets"
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

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG", "AE_PROFILING" }
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines { "AE_RELEASE", "AE_RELEASE_PROFILING" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        flags { "LinkTimeOptimization" }
        runtime "Release"
        optimize "on"
