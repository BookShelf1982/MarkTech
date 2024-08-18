project "Gameplay"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	files { "Private/**.cpp", "Private/**.h", "Public/**.h" }
	
	includedirs {
		"Public/",
		IncludeDirs["Shared"],
		IncludeDirs["Core"],
		IncludeDirs["ResourceManager"],
		IncludeDirs["Graphics"],
		IncludeDirs["FileSystem"]
	}
	links { "Core", "ResourceManager", "Graphics" }
	
	filter "configurations:Debug"
       defines { "DEBUG", "MT_ENABLE_ASSERTS" }
       symbols "On"
		
	filter "configurations:Release"
		defines { "NDEBUG" }
        optimize "On"
		
	filter "platforms:Win64"
		defines { "MT_PLATFORM_WINDOWS" }