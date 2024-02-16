project "EngineCore"
    kind "StaticLib"
    language "C++"
	cppdialect "C++17"
	
    files { "Source/**.h", "Source/**.cpp", "Source/**.c" }
	
    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{wks.location}/Shared/",
		"%{wks.location}/Imgui/Source/",
		"C:/VulkanSDK/1.3.275.0/Include/"
	}
	
	libdirs
	{
		"C:/VulkanSDK/1.3.275.0/Lib/"
	}
	
	defines
    {
        "MT_PLATFORM_WINDOWS"
    }
	
	links
	{
		"Imgui",
		"volk.lib"
	}
	
    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"