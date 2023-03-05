workspace "AnEngine"
    architecture "x64"
    configurations { "Debug", "Release", "Dist" }

outputDir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "AnEngine"
    location "AnEngine"
    kind "SharedLib"
    language "C++"
    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }
    includedirs { 
        "%{prj.name}/src/AnEngine/include/",
        "%{prj.name}/vendor/spdlog/include/"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

        defines { 
            "AE_WIN",
            "AE_DLL"
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

    filter { "system:windows", "configurations:Release" }
        buildoptions { "/MT", "/W4" }

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp"
    language "C++"
    links { "AnEngine" }

    targetdir ("bin/" .. outputDir .. "/%{prj.name}")
    objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

    files { "%{prj.name}/src/**.hpp", "%{prj.name}/src/**.cpp" }

    includedirs { 
        "%{prj.name}/src/SandBox/include/",
        "AnEngine/src",
        "AnEngine/vendor/spdlog/include/"
    }

    filter "system:windows"
        cppdialect "C++20"
        staticruntime "On"
        systemversion "latest"

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

    filter { "system:windows", "configurations:Release" }
        buildoptions { "/MT", "/W3" }