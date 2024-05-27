workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")
	
	IncludeDirs = {}
	IncludeDirs["Shared"] = "%{wks.location}/Engine/Source/Runtime/Shared/"
	IncludeDirs["Core"] = "%{wks.location}/Engine/Source/Runtime/Core/Public/"
	IncludeDirs["DSA"] = "%{wks.location}/Engine/Source/Runtime/DSA/Public/"
	IncludeDirs["FileSystem"] = "%{wks.location}/Engine/Source/Runtime/FileSystem/Public/"
	IncludeDirs["HighResTimer"] = "%{wks.location}/Engine/Source/Runtime/HighResTimer/Public/"

	
	include "Engine/Source/Runtime/Core/"
	include "Engine/Source/Runtime/DSA/"
	include "Engine/Source/Runtime/Sandbox/"
	include "Engine/Source/Runtime/FileSystem/"
	include "Engine/Source/Runtime/HighResTimer/"