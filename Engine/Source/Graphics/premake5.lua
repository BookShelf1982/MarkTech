project "Graphics"
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
		"%{wks.location}/Engine/ThirdParty/GLFW/include/"
	}
	
	libdirs
	{
		"%{wks.location}/Engine/THirdParty/GLFW/lib/"
	}
	
	links
	{
		"glfw3.lib"
	}
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		
	filter "configurations:Release"
        optimize "On"