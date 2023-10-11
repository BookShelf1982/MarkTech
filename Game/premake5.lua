project "Game"
    kind "WindowedApp"
    language "C++"
	
	IncludeDirs = {}
	IncludeDirs["Engine"] = "%{EngineDir}/Source"
	IncludeDirs["spdlog"] = "%{EngineDir}/ThirdParty/spdlog-1.x/include"
	
	include "Engine"
	
    files { "Source/**.h", "Source/**.cpp" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{IncludeDirs.Engine}",
		"%{IncludeDirs.spdlog}",
	}
	
	links
	{
		"Engine"
	}
	
	defines
    {
        "MT_PLATFORM_WINDOWS",
        "MT_ENABLE_ASSERTS"
    }
	
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"