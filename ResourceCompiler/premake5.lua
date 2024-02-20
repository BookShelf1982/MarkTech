project "ResourceCompiler"
    kind "ConsoleApp"
    language "C++"

    files 
	{
		"Source/**.h", 
		"Source/**.cpp"
	}
	
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	
	includedirs
	{
		"%{prj.location}/ThirdParty/Assimp/include",
		"C:/VulkanSDK/1.3.275.0/Include"
	}
	
	libdirs
	{
		"%{prj.location}/ThirdParty/Assimp/lib/x64",
		"C:/VulkanSDK/1.3.275.0/Lib"
	}
	
	links
	{
		"assimp-vc143-mt.lib",
		"shaderc_combined.lib",
	}
	
	postbuildcommands
	{
		"mkdir %{wks.location}Build\\Bin",
		"{COPYFILE} %{wks.location}bin/" .. outputdir .. "/%{prj.name}/ResourceCompiler.exe %{wks.location}Build/Bin",
		"{COPYFILE} %{prj.location}ThirdParty/Assimp/lib/x64/assimp-vc143-mt.dll %{wks.location}Build/Bin"
	}

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "NDEBUG" }
        optimize "On"