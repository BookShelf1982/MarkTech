project "Engine"
    kind "StaticLib"
    language "C++"

    IncludeDirs = {}
	IncludeDirs["spdlog"] = "%{prj.location}/ThirdParty/spdlog-1.x/include"

    files { "Source/**.h", "Source/**.cpp" }
	
	includedirs
	{
		"%{IncludeDirs.spdlog}"
	}
	
    defines
    {
        "MT_PLATFORM_WINDOWS",
        "MT_ENABLE_ASSERTS"
    }

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")


    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"