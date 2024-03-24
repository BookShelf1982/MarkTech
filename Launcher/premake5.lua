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
		"%{wks.location}/Engine/Source/Core/Public/",
		"%{wks.location}/Engine/Source/Global/Public/"
	}
	
	links
	{
		"Core"
	}
	
	postbuildcommands
	{
		"mkdir %{wks.location}Build\\Bin",
		"{COPYFILE} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/%{prj.name}.exe %{wks.location}/Build/"
	}
	
	defines {"MT_PLATFORM_WINDOWS"}
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		debugdir "%{wks.location}/Build/"
		
	filter "configurations:Release"
        optimize "On"