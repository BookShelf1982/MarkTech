project "SandboxApp"
	kind "WindowedApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "Private/**.cpp", "Private/**.h", "Public/**.h" }
	
	includedirs { 
		"Public/",
		"Private/"
	}
	
	filter "configurations:Debug"
       defines { "DEBUG", "MT_ENABLE_ASSERTS" }
       symbols "On"
	   debugdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		
	filter "platforms:Win64"
		defines { "MT_PLATFORM_WINDOWS" }