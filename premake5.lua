workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")
	
	IncludeDirs = {}
	IncludeDirs["Core"] = "%{wks.location}/Engine/Source/Runtime/Core/Public/"
	IncludeDirs["DSA"] = "%{wks.location}/Engine/Source/Runtime/DSA/Public/"
	
	include "Engine/Source/Runtime/Core/"
	include "Engine/Source/Runtime/DSA/"
	include "Engine/Source/Runtime/Sandbox/"