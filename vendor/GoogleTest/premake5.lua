project "googletest"
	location "vendor/googletest"
	language "C++"
	systemversion "latest"
	kind "StaticLib"
	staticruntime "off"
	

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../build/" .. outputdir .. "/%{prj.name}")

	includedirs 
	{
		"googletest/include/",
		"googletest/"
	}

	files 
	{ 
		"googletest/src/gtest-all.cc"
	}
	
		filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"