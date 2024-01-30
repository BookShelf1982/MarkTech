project "Game"
    kind "WindowedApp"
    language "C++"
	
    files { "Source/**.h", "Source/**.cpp" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Engine/EngineCore/Source",
		"%{wks.location}/Engine/EngineCore/ThirdParty/DirectXHelpers",
		"%{wks.location}/Shared"
	}
	
	defines
    {
        "MT_PLATFORM_WINDOWS",
    }
	
	postbuildcommands
	{
		"{COPYFILE} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/Game.exe %{wks.location}/Build/"
	}
	
	links
	{
		"EngineCore"
	}
	
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"