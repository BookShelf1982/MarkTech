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
		"%{prj.name}/3rdparty/spdlog/include",
		"%{prj.name}/src",
		"%{prj.name}/src/MarkTech",
		"%{prj.name}/3rdparty/GLFW/include"
	}

	libdirs
	{
		"%{prj.name}/3rdparty/GLFW/lib-vc2019"
	}

	links
	{
		"glfw3.lib",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {"MT_PLATFORM_WINDOWS", "MT_BUILD_DLL", "MT_ENABLE_ASSERTS","WIN32"}

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
