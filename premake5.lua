workspace "Crystal"
	architecture "x64"
	startproject "Sapphire-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Crystal/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Crystal/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/Crystal/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/Crystal/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/Crystal/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/Crystal/vendor/entt"
IncludeDir["yaml"] = "%{wks.location}/Crystal/vendor/yaml/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/Crystal/vendor/ImGuizmo"

group "Dependencies"
	include "Crystal/vendor/GLFW"
	include "Crystal/vendor/Glad"
	include "Crystal/vendor/imgui"
	include "Crystal/vendor/yaml"
group ""

project "Crystal"
	location "Crystal"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "crystalpch.h"
	pchsource "Crystal/src/crystalpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
	}

	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.yaml}",
		"%{IncludeDir.ImGuizmo}"
	}

	links 
	{ 
		"GLFW",
		"Glad",
		"ImGui",
		"yaml-cpp",
		"opengl32.lib"
	}

	filter "files:vendor/ImGuizmo/**.cpp"
		flags { "NoPCH" }

	filter "system:windows"
		systemversion "latest"

		defines
		{
			-- "CRYSTAL_PLATFORM_WINDOWS",
			"CRYSTAL_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CRYSTAL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "on"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Crystal/src",
		"Crystal/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}"
	}

	links
	{
		"Crystal"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CRYSTAL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "on"


project "Sapphire-Editor"
	location "Sapphire-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

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
		"Crystal/src",
		"Crystal/vendor",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.ImGuizmo}"
	}

	links
	{
		"Crystal"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"CRYSTAL_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "CRYSTAL_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "CRYSTAL_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "CRYSTAL_DIST"
		runtime "Release"
		optimize "on"