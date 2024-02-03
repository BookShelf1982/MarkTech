-- premake5.lua
workspace "MarkTech"
	configurations { "Debug", "Release" }
	architecture "x86_64"
	
	outputdir = ("%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}")

	include "Editor/"
	include "Game/"
	include "Imgui/"
	include "ResourceCompiler/"
	include "DirectXTex/"
	include "Engine/EngineCore/"
	include "D3D12MemoryAllocator"