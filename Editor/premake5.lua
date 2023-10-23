project "Editor"
    kind "ConsoleApp"
    language "C++"
	
	include "Engine"
	
    files { "Source/**.h", "Source/**.cpp" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Engine/Source",
		"%{wks.location}/Imgui/Source"
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