workspace "AnEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

includeDir = {}
includeDir['GLFW'] = "AnEngine/vendor/GLFW/include"
includeDir['Glad'] = "AnEngine/vendor/Glad/include"
includeDir['ImGui'] = "AnEngine/vendor/ImGui/"

include "AnEngine/vendor/GLFW"
include "AnEngine/vendor/Glad"
include "AnEngine/vendor/ImGui"

project "AnEngine"
    location "AnEngine"
    kind "SharedLib"
    language "C++"
    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    pchheader "aepch.hpp"
    pchsource "AnEngine/src/aepch.cpp"

    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }
    includedirs { 
        "%{prj.name}/src/AnEngine/include/",
        "%{prj.name}/src/Platform/Windows/include/",
        "%{prj.name}/src/Platform/OpenGL/include/",
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include/",
        "%{includeDir.GLFW}",
        "%{includeDir.Glad}",
        "%{includeDir.ImGui}"
    }

    links {
        "GLFW",
        "Glad",
        "ImGui",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/external:W0" }

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
        symbols "On"

    filter "configurations:Release"
        defines { "AE_RELEASE" }
        optimize "On"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        optimize "On"

    filter { "system:windows", "configurations:Debug" }
        links {
            "msvcrtd.lib"
        }
        buildoptions { "/MDd" }

    filter { "system:windows", "configurations:Release" }
        links {
            "msvcrt.lib"
        }
        buildoptions { "/MD", "/W4" }

    filter { "system:windows", "configurations:Dist" }
        links {
            "msvcrt.lib"
        }
        buildoptions { "/MD", "/W4" }


project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    links { "AnEngine" }

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }

    includedirs { 
        "%{prj.name}/src/include/",
        "AnEngine/src",
        "AnEngine/vendor/spdlog/include/"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"
        buildoptions { "/external:W0" }

        defines { 
            "AE_WIN",
        }

    filter "configurations:Debug"
        defines { "AE_DEBUG_FLAG", "_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "AE_RELEASE" }
        optimize "On"

    filter "configurations:Dist"
        defines { "AE_DIST" }
        optimize "On"

    filter { "system:windows", "configurations:Debug" }
        links {
            "msvcrtd.lib"
        }

    filter { "system:windows", "configurations:Release" }
        links {
            "msvcrt.lib"
        }
        buildoptions { "/W4" }

    filter { "system:windows", "configurations:Dist" }
        links {
            "msvcrt.lib"
        }
        buildoptions { "/W4" }