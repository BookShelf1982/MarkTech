workspace "MarkTech"
	architecture "x86"
	configurations {"Debug", "Release", "Shipping"}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
		"%{prj.name}/3rdparty/spdlog/include"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {"MT_PLATFORM_WINDOWS", "MT_BUILD_DLL", "WIN32"}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Game")
		}

	filter "configurations:Debug"
		defines "MT_DEBUG"
		symbols "On"

	filter "configurations:Release"
		defines "MT_RELEASE"
		optimize "On"

	filter "configurations:Shipping"
		defines "MT_SHIPPING"
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
		symbols "On"

	filter "configurations:Release"
		defines "MT_RELEASE"
		optimize "On"

	filter "configurations:Shipping"
		defines "MT_SHIPPING"
		optimize "On"

	filter {"system:windows", "configurations:Release", "configurations:Shipping"}
		buildoptions "/MT"
