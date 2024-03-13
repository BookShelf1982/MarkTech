workspace "MarkTech"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")

	include "Launcher/"
	include "Engine/Source/Core/"
	include "Engine/Source/Graphics/"