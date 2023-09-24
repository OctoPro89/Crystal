workspace "Crystal"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

--Include Directories relative root folder (Solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "Crystal/vendor/GLFW/include"

include "Crystal/vendor/GLFW"

project "Crystal"
	location "Crystal"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crystalpch.h"
	pchsource "Crystal/src/crystalpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Crystal/src",
		"Crystal/vendor/spdlog/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	postbuildcommands
	{
		
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS",
			"CRYSTAL_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CRYSTAL_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"Crystal/vendor/spdlog/include",
		"Crystal/src"
	}

	links
	{
		"Crystal"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "CRYSTAL_Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		optimize "On"