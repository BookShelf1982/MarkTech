workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")

	include "Launcher/"
	include "Engine/Source/Core/"
	include "Engine/Source/Graphics/"
	include "Engine/Source/Window/"
	include "Engine/Source/ResourceCompiler"
	include "Engine/ThirdParty/Glad/"