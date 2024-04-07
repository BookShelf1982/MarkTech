project "ResourceCompiler"
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
		"C:/VulkanSDK/1.3.275.0/Include/"
	}
	
	libdirs
	{
		"C:/VulkanSDK/1.3.275.0/Lib"
	}
	
	links
	{
		"shaderc.lib"
	}
	
	postbuildcommands
	{
		"mkdir %{wks.location}Build\\Bin\\",
		"{COPYFILE} %{wks.location}/bin/" .. outputdir .. "/%{prj.name}/%{prj.name}.dll %{wks.location}/Build/Bin/"
	}
	
	filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		
	filter "configurations:Release"
        optimize "On"
		
	filter "platforms:Win64"
		defines {"MT_PLATFORM_WINDOWS"}