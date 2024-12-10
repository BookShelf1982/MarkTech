workspace "MarkTech"
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")
	
	IncludeDirs = {}
	--IncludeDirs["Shared"] = "%{wks.location}/Engine/Source/Runtime/Shared/"
	--IncludeDirs["nbt"] = "%{wks.location}/Engine/Source/Runtime/nbt/Public/"
	--[[IncludeDirs["Platform"] = "%{wks.location}/Engine/Source/Runtime/Platform/Public/"
	IncludeDirs["MarkPak"] = "%{wks.location}/Engine/Source/Tools/MarkPak/Public/"
	IncludeDirs["Volk"] = "%{wks.location}/Engine/ThirdParty/Volk/"
	IncludeDirs["stb"] = "%{wks.location}/Engine/ThirdParty/stb/"
	IncludeDirs["zlib"] = "%{wks.location}/Engine/ThirdParty/zlib/Public/"
	IncludeDirs["VULKAN"] = "C:\\VulkanSDK\\1.3.275.0\\Include"]]--

	include "Engine/Source/Runtime/Sandbox/"
	--[[include "Engine/Source/Runtime/nbt/"
	include "Engine/Source/Runtime/Platform/"
	include "Engine/Source/Tools/MarkPak/"
	include "Engine/ThirdParty/Volk/"
	include "Engine/ThirdParty/zlib/"]]--