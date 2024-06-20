workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")
	
	IncludeDirs = {}
	IncludeDirs["Shared"] = "%{wks.location}/Engine/Source/Runtime/Shared/"
	IncludeDirs["Core"] = "%{wks.location}/Engine/Source/Runtime/Core/Public/"
	IncludeDirs["ResourceManager"] = "%{wks.location}/Engine/Source/Runtime/ResourceManager/Public/"
	IncludeDirs["FileSystem"] = "%{wks.location}/Engine/Source/Runtime/FileSystem/Public/"
	IncludeDirs["HighResTimer"] = "%{wks.location}/Engine/Source/Runtime/HighResTimer/Public/"
	IncludeDirs["Engine"] = "%{wks.location}/Engine/Source/Runtime/Engine/Public/"
	IncludeDirs["MarkPak"] = "%{wks.location}/Engine/Source/Tools/MarkPak/Public/"
	--IncludeDirs["ResourceCompiler"] = "%{wks.location}/Engine/Source/Tools/ResourceCompiler/Public/"

	
	include "Engine/Source/Runtime/Core/"
	include "Engine/Source/Runtime/Sandbox/"
	include "Engine/Source/Runtime/FileSystem/"
	include "Engine/Source/Runtime/HighResTimer/"
	include "Engine/Source/Runtime/Engine/"
	include "Engine/Source/Runtime/ResourceManager/"
	include "Engine/Source/Tools/MarkPak/"
	include "Engine/Source/Tools/ResourceCompiler/"