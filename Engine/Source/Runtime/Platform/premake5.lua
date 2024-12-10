project "Platform"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "Private/**.c", "Private/**.h", "Public/**.h" }
	includedirs { "Public/", IncludeDirs["Shared"] }

	filter "configurations:Debug"
       defines { "DEBUG", "MT_ENABLE_ASSERTS" }
       symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		symbols "Off"
		
	filter "platforms:Win64"
		defines { "MT_PLATFORM_WINDOWS" }
		
	filter "platforms:OSX"
		defines { "MT_PLATFORM_OSX" }