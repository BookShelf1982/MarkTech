project "Engine"
    kind "StaticLib"
    language "C++"

    files { "Source/**.h", "Source/**.cpp"}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Imgui/Source"
	}
	
    defines
    {
        "MT_PLATFORM_WINDOWS",
        "MT_ENABLE_ASSERTS",
    }
	
	links
	{
		"d3d11.lib",
		"Imgui"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"