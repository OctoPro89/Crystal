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
	include "Crystal/vendor/Box2D"
	include "Crystal/vendor/GLFW"
	include "Crystal/vendor/Glad"
	include "Crystal/vendor/msdf-atlas-gen"
	include "Crystal/vendor/imgui"
	include "Crystal/vendor/yaml"
group ""

group "Core"
	include "Crystal"
	include "Crystal-ScriptCore"
group ""

group "Tools"
	include "Sapphire-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""
