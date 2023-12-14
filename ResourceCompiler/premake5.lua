project "ResourceCompiler"
    kind "SharedLib"
    language "C++"

    files 
	{
		"Source/**.h", 
		"Source/**.cpp",
		"ThirdParty/**.h",
		"ThirdParty/**.cpp",
		"ThirdParty/**.inl"
	}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{prj.location}/ThirdParty/"
	}
	
    defines
    {
        "MT_PLATFORM_WINDOWS",
    }
	
	links
	{
	}
	
	postbuildcommands
	{
		"{MKDIR} %{wks.location}Build/Bin/",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/ResourceCompiler.dll %{wks.location}/Build/Bin"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"