workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")
	
	IncludeDirs = {}
	IncludeDirs["Shared"] = "%{wks.location}/Engine/Source/Runtime/Shared/"
	IncludeDirs["Core"] = "%{wks.location}/Engine/Source/Runtime/Core/Public/"
	IncludeDirs["Gameplay"] = "%{wks.location}/Engine/Source/Runtime/Gameplay/Public/"
	IncludeDirs["Input"] = "%{wks.location}/Engine/Source/Runtime/Input/Public/"
	IncludeDirs["ResourceManager"] = "%{wks.location}/Engine/Source/Runtime/ResourceManager/Public/"
	IncludeDirs["FileSystem"] = "%{wks.location}/Engine/Source/Runtime/FileSystem/Public/"
	IncludeDirs["HighResTimer"] = "%{wks.location}/Engine/Source/Runtime/HighResTimer/Public/"
	IncludeDirs["Threads"] = "%{wks.location}/Engine/Source/Runtime/Threads/Public/"
	IncludeDirs["Graphics"] = "%{wks.location}/Engine/Source/Runtime/Graphics/Public/"
	IncludeDirs["nbt"] = "%{wks.location}/Engine/Source/Runtime/nbt/Public/"
	IncludeDirs["MarkPak"] = "%{wks.location}/Engine/Source/Tools/MarkPak/Public/"
	IncludeDirs["Volk"] = "%{wks.location}/Engine/ThirdParty/Volk/"
	IncludeDirs["stb"] = "%{wks.location}/Engine/ThirdParty/stb/"
	IncludeDirs["zlib"] = "%{wks.location}/Engine/ThirdParty/zlib/Public/"
	IncludeDirs["VULKAN"] = "C:\\VulkanSDK\\1.3.275.0\\Include"

	
	include "Engine/Source/Runtime/Core/"
	include "Engine/Source/Runtime/Sandbox/"
	include "Engine/Source/Runtime/Gameplay/"
	include "Engine/Source/Runtime/Input/"
	include "Engine/Source/Runtime/FileSystem/"
	include "Engine/Source/Runtime/HighResTimer/"
	include "Engine/Source/Runtime/Threads/"
	include "Engine/Source/Runtime/ResourceManager/"
	include "Engine/Source/Tools/MarkPak/"
	include "Engine/Source/Runtime/Graphics/"
	include "Engine/Source/Runtime/nbt/"
	include "Engine/ThirdParty/Volk/"
	include "Engine/ThirdParty/zlib/"