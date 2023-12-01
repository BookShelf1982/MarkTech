project "ResourceCompilerCmd"
    kind "ConsoleApp"
    language "C++"

    files 
	{
		"Source/**.h", 
		"Source/**.cpp",
	}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	links
	{
	}
	
	postbuildcommands
	{
		"{MKDIR} %{wks.location}Build/Bin/",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/ResourceCompilerCmd.exe %{wks.location}/Build/Bin"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"