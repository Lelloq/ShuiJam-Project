workspace "ShuiJam"
	architecture "x64"
	startproject "ShuiJam"

	configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}"

group "ShuiJam"

project "ShuiJam"
	location "ShuiJam"
	kind "ConsoleApp"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{	
		"SJEngine/code/include",
		"%{prj.name}/include",
		"vendor/glfw/include",
		"vendor/glm/",
		"vendor/spdlog/include",
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "On"

project "SJEngine"
	location "SJEngine"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "SJEngine/precompiled/pch.cpp"
	
	files
	{
		"%{prj.name}/code/**.h",
		"%{prj.name}/code/**.cpp",
		"%{prj.name}/precompiled/pch.h",
		"%{prj.name}/precompiled/pch.cpp",
		"%{prj.name}/shader/vert/**.vert",
		"%{prj.name}/shader/frag/**.frag"
	}

	includedirs
	{
		"%{prj.name}/code/",
		"%{prj.name}/code/include/",
		"%{prj.name}/precompiled/",
		"vendor/spdlog/include",
		"vendor/glfw/include",
		"vendor/Glad/include",
		"vendor/glm/",
		"vendor/STBimage",
		"vendor/freetype2/include",
		"vendor/OpenAL-Soft/includeAL",
		"vendor/ZipLib/Source/ZipLib"
	}
	
	links 
	{
		"GLFW",
		"Glad",
		"Freetype",
		"OpenAL"
	}
	
	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "On"

group "Vendor"
	include "vendor/glfw"
	include "vendor/googletest"
	include "vendor/Glad"
	include "vendor/freetype2"
	include "vendor/OpenAL-Soft"
