project "zlib"
	kind "StaticLib"
	language "C"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "Private/**.c", "Private/**.h", "Public/**.h" }
	includedirs { "Public/" }

	filter "configurations:Debug"
       defines { "DEBUG", "MT_ENABLE_ASSERTS" }
       symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		
	filter "platforms:Win64"
		defines { "MT_PLATFORM_WINDOWS" }