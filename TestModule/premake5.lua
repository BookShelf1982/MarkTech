project "TestModule"
    kind "SharedLib"
    language "C++"

    files 
	{
		"Source/**.h", 
		"Source/**.cpp",
		"Source/**.c"
	}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Shared",
		"ThirdParty/Lua/include"
	}
	
	links
	{
		"ThirdParty/Lua/lua54.lib"
	}
	
	postbuildcommands
	{
		"{MKDIR} %{wks.location}Build/Bin/Modules/",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/TestModule.dll %{wks.location}/Build/Bin/Modules/"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"