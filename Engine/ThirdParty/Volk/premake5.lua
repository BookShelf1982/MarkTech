project "Volk"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "volk.c", "volk.h" }
	
	includedirs { IncludeDirs["VULKAN"] }
	
	defines { "VOLK_STATIC_DEFINES" }
	
	filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		
	filter "platforms:Win64"
		defines { "VK_USE_PLATFORM_WIN32_KHR" }