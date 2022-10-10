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

project "SJRenderer"
	location "SJRenderer"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "SJRenderer/precompiled/pch.cpp"
	
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
	}
	
	links 
	{
		"GLFW",
		"Glad",
		"Freetype",
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
