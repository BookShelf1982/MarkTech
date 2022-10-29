workspace "MarkTech"
	architecture "x86"
	configurations {"Debug", "Release", "Shipping"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDirs = {}
IncludeDirs["GLFW"] = "MarkTech/3rdparty/GLFW/include"
IncludeDirs["Glad"] = "MarkTech/3rdparty/Glad/include"

include "MarkTech/3rdparty/Glad"

project "MarkTech"
	location "MarkTech"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/src/MarkTech",
		"%{prj.name}/3rdparty/spdlog/include",
		"%{IncludeDirs.GLFW}",
		"%{IncludeDirs.Glad}"
	}

	libdirs
	{
		"%{prj.name}/3rdparty/GLFW/lib-vc2019"
	}

	links
	{
		"Glad",
		"glfw3.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {"MT_PLATFORM_WINDOWS", 
		"MT_BUILD_DLL", 
		"MT_ENABLE_ASSERTS",
		"GLFW_INCLUDE_NONE",
		"WIN32"}

	filter "configurations:Debug"
		defines "MT_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MT_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Shipping"
		defines "MT_SHIPPING"
		buildoptions "/MD"
		optimize "On"

	filter {"system:windows", "configurations:Release", "configurations:Shipping"}
		buildoptions "/MT"

project "Game"
	location "Game"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"$(SolutionDir)MarkTech/3rdparty/spdlog/include",
		"$(SolutionDir)MarkTech/src"
	}

	links
	{
		"MarkTech"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {"MT_PLATFORM_WINDOWS", "WIN32"}

	filter "configurations:Debug"
		defines "MT_DEBUG"
		buildoptions "/MDd"
		symbols "On"

	filter "configurations:Release"
		defines "MT_RELEASE"
		buildoptions "/MD"
		optimize "On"

	filter "configurations:Shipping"
		defines "MT_SHIPPING"
		buildoptions "/MD"
		optimize "On"
