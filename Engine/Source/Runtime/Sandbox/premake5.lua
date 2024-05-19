project "SandboxApp"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "Private/**.cpp", "Private/**.h" }
	includedirs { "Public/", IncludeDirs["DSA"], IncludeDirs["FileSystem"] }
	
	links { "DSA", "FileSystem" }
	
	filter "configurations:Debug"
       defines { "DEBUG" }
       symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		
	filter "platforms:Win64"
		defines { "MT_PLATFORM_WINDOWS" }