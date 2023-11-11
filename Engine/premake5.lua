project "Engine"
    kind "SharedLib"
    language "C++"

    files 
	{
		"Source/**.h", 
		"Source/**.cpp",
		"ThirdParty/**.h",
		"ThirdParty/**.cpp"
	}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Imgui/Source",
		"%{wks.location}/Inipp/Source",
		"%{prj.location}/ThirdParty"
	}
	
    defines
    {
        "MT_PLATFORM_WINDOWS",
		"MT_BUILD_DLL",
		"_CRT_SECURE_NO_WARNINGS"
    }
	
	links
	{
		"d3d11.lib",
		"Imgui"
	}
	
	postbuildcommands
	{
		"{COPYFILE} %{prj.location}Source/Shaders/ %{wks.location}/Build/Bin/Shaders/Source/",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/Engine.dll %{wks.location}/Build/Bin"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"