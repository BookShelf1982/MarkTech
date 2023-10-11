-- premake5.lua
workspace "MarkTech"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")
	
	EngineDir = ("%{wks.location}/Engine")

	include "Engine/"
	include "Game/"
	include "IMGUI/"
	include "ResourceCompiler/"