project "Window"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"Public/**.h",
		"Private/**.cpp",
		"Private/**.h"
	}
	
	includedirs
	{
		"Public/",
		"%{wks.location}/Engine/Source/Global/Public/",
		"%{wks.location}/Engine/ThirdParty/GLFW/include/",
	}
	
	libdirs
	{
		"%{wks.location}/Engine/THirdParty/GLFW/lib/"
	}
	
	links
	{
		"glfw3.lib"
	}
	
	postbuildcommands
	{
		"mkdir %{wks.location}Build\\Bin",
		"{COPYFILE} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/%{prj.name}.dll %{wks.location}/Build/Bin/"
	}
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		
	filter "configurations:Release"
        optimize "On"
		
	filter "platforms:Win64"
		defines {"MT_PLATFORM_WINDOWS"}