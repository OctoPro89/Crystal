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
IncludeDir["Glad"] = "Crystal/vendor/Glad/include"
IncludeDir["ImGui"] = "Crystal/vendor/imgui"

include "Crystal/vendor/GLFW"
include "Crystal/vendor/Glad"
include "Crystal/vendor/imgui"

project "Crystal"
	location "Crystal"
	kind "SharedLib"
	language "C++"
	staticruntime "off"

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
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS",
			"CRYSTAL_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CRYSTAL_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "On"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

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
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "CRYSTAL_Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "On"