project "ResourceCompiler"
    kind "SharedLib"
    language "C++"

    files 
	{
		"Source/**.h", 
		"Source/**.cpp",
		"ThirdParty/**.h",
		"ThirdParty/**.cpp",
		"ThirdParty/**.inl"
	}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{prj.location}/ThirdParty/",
		"%{prj.location}/ThirdParty/FBXSDK/include/"
	}
	
	libdirs
	{
		"%{prj.location}/ThirdParty/FBXSDK/lib/vs2022/x64/",
	}
	
	postbuildcommands
	{
		"{MKDIR} %{wks.location}Build/Bin/",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/ResourceCompiler.dll %{wks.location}/Build/Bin"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"
		staticruntime "on"
		runtime "Debug"
		links {"debug/libfbxsdk-mt.lib", "debug/libxml2-mt.lib", "debug/zlib-mt.lib"}

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"
		staticruntime "on"
		runtime "Release"
		links {"release/libfbxsdk-mt.lib", "release/libxml2-mt.lib", "release/zlib-mt.lib"}