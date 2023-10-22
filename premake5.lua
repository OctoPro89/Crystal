include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

workspace "Crystal"
	architecture "x86_64"
	startproject "Sapphire-Editor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

group "Dependencies"
	include "vendor/premake"
	include "Crystal/vendor/GLFW"
	include "Crystal/vendor/Glad"
	include "Crystal/vendor/imgui"
	include "Crystal/vendor/yaml"
group ""

include "Crystal"
include "Sandbox"
include "Sapphire-Editor"