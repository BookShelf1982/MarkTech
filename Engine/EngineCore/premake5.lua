project "EngineCore"
    kind "SharedLib"
    language "C++"
	
    files { "Source/**.h", "Source/**.cpp" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Shared"
	}
	
	defines
    {
        "MT_PLATFORM_WINDOWS",
    }
	
	postbuildcommands
	{
		"{MKDIR} %{wks.location}/Build/Bin/Modules/"
		"{COPYFILE} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/EngineCore.dll %{wks.location}/Build/Bin/Modules/"
	}
	
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"