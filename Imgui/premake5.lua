project "Imgui"
    kind "StaticLib"
    language "C++"

    files { "Source/**.h", "Source/**.cpp"}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
    defines
    {
        "MT_PLATFORM_WINDOWS"
    }
	
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"