project "ZipLib"
	kind "StaticLib"
	language "C++"
	cppdialect "C++11"
	staticruntime "on"
		systemversion "latest"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../build/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"Source/ZipLib",
		"Source/ZipLib/detail",
	}

	files 
	{
		"Source/ZipLib/**.h",
		"Source/ZipLib/**.cpp"
	}

	    filter "system:windows"
        	systemversion "latest"

	    filter "configurations:Debug"
			runtime "Debug"
	       	symbols "on"

    	filter "configurations:Release"
        	runtime "Release"
        	optimize "on"