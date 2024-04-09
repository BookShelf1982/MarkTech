workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")

	include "Launcher/"
	include "ResourceCompilerCmd/"
	include "Engine/Source/Core/"
	include "Engine/Source/Standard/"
	include "Engine/Source/ResourceManager/"
	include "Engine/Source/Graphics/"
	include "Engine/Source/Window/"
	include "Engine/Source/ResourceCompiler"
	include "Engine/ThirdParty/Glad/"