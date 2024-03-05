-- premake5.lua
workspace "MarkTech"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")

	include "Game/"
	include "Engine/EngineCore/ThirdParty/Imgui/"
	include "ResourceCompiler/"
	include "Engine/EngineCore/"