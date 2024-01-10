project "EngineCore"
    kind "StaticLib"
    language "C++"
	
    files { "Source/**.h", "Source/**.cpp" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Shared/",
		"%{wks.location}/Imgui/Source/"
	}
	
	defines
    {
        "MT_PLATFORM_WINDOWS",
    }
	
	links
	{
		"Imgui"
	}
	
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"