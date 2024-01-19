project "Glad"
	kind "StaticLib"
    staticruntime "off"
	language "C"

	targetdir ("bin/" .. outputDir .. "/%{prj.name}")
	objdir ("bin/intermediate/" .. outputDir .. "/%{prj.name}")

	files {
		"include/glad/glad.h",
        "include/KHR/khrplatform.h",
		"src/glad.c",
	}

    includedirs {
        'include'
    }

	filter "system:windows"
        systemversion "latest"

    filter "system:linux"
        pic "On"
        systemversion "latest"
        staticruntime "On"

    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        runtime "Release"
        optimize "on"