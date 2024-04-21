project "Core"
	kind "StaticLib"
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
	
	links
	{
		"Standard"
	}
	
	includedirs
	{
		"Public/",
		"%{wks.location}/Engine/Source/Standard/Public/",
		"%{wks.location}/Engine/ThirdParty/",
		"%{wks.location}/Engine/Source/Window/Public/",
		"%{wks.location}/Engine/Source/Graphics/Public/",
		"%{wks.location}/Engine/Source/ResourceManager/Public/",
		"%{wks.location}/Engine/ThirdParty/GLFW/include/"
	}
	
	filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
		
	filter "configurations:Release"
        optimize "On"
		
	filter "platforms:Win64"
		defines {"MT_PLATFORM_WINDOWS"}