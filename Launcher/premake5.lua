project "Launcher"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"Source/**.h",
		"Source/**.cpp",
	}
	
	includedirs
	{
		"%{wks.location}/Engine/Source/Core/Public"
	}
	
	links
	{
		"Core"
	}
	
	defines {"MT_PLATFORM_WINDOWS"}
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		
	filter "configurations:Release"
        optimize "On"