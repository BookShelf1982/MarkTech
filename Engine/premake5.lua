project "Engine"
    kind "StaticLib"
    language "C++"

    IncludeDirs = {}
	IncludeDirs["inipp"] = "%{prj.location}/ThirdParty/inipp/"

    files { "Source/**.h", "Source/**.cpp", "Source/Shaders/**.hlsl"}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		
	}
	
    defines
    {
        "MT_PLATFORM_WINDOWS",
        "MT_ENABLE_ASSERTS"
    }
	
	links
	{
		"d3d11.lib",
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"