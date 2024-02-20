project "Game"
    kind "WindowedApp"
    language "C++"
	cppdialect "C++17"
	
    files { "Source/**.h", "Source/**.cpp" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Engine/EngineCore/Source",
		"C:/VulkanSDK/1.3.275.0/Include"
	}
	
	defines
    {
        "MT_PLATFORM_WINDOWS",
    }
	
	postbuildcommands
	{
		"mkdir %{wks.location}Build\\Bin",
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