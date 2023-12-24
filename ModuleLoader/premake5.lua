project "ModuleLoader"
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
		"%{wks.location}/Shared/"
	}
	
	postbuildcommands
	{
		"{MKDIR} %{wks.location}Build/Bin/",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/ModuleLoader.dll %{wks.location}/Build/Bin"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"