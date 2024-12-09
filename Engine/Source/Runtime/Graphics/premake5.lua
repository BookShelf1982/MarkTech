project "Graphics"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "Private/**.cpp", "Private/**.h", "Public/**.h" }
	includedirs { "Public/", IncludeDirs["Shared"], IncludeDirs["Volk"], IncludeDirs["VULKAN"], IncludeDirs["Core"], IncludeDirs["ResourceManager"] }
	
	links { "Volk", "Core", "ResourceManager" }
	
	filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		
	filter "platforms:Win64"
		defines { "MT_PLATFORM_WINDOWS" }
		
	filter "platforms:OSX"
		defines { "MT_PLATFORM_OSX" }