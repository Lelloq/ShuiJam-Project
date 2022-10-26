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
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("build/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "ShuiJam/Precompiled/pch.cpp"

	files
	{
		"%{prj.name}/Game/include/**.h",
		"%{prj.name}/Game/src/**.cpp",
		"%{prj.name}/Renderer/include/**.h",
		"%{prj.name}/Renderer/src/**.cpp",
		"%{prj.name}/Precompiled/pch.h",
		"%{prj.name}/Precompiled/pch.cpp",
		"%{prj.name}/Renderer/shader/vert/**.vert",
		"%{prj.name}/Renderer/shader/frag/**.frag"
	}

	includedirs
	{	
		"%{prj.name}/Renderer/include",
		"%{prj.name}/Game/include",
		"%{prj.name}/Game",
		"%{prj.name}/Precompiled/",
		"vendor/glfw/include/",
		"vendor/glm/",
		"vendor/Glad/include/",
		"vendor/STBimage",
		"vendor/freetype2/include",
		"vendor/OpenAL-Soft/include",
		"vendor/OpenAL-Soft/src",
		"vendor/OpenAL-Soft/src/common/",
		"vendor/minimp3/",
		"vendor/libogg/include/",
		"vendor/Vorbis/include/",
		"vendor/drwav/",
		"vendor/sqlite/",
		"vendor/bit7z/include"
	}

	links 
	{
		"GLFW",
		"Glad",
		"Freetype",
		"OpenAL-Soft",
		"Vorbis",
		"bit7z",
	}

	defines
	{
		"AL_LIBTYPE_STATIC"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

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
	include "vendor/Vorbis"
	include "vendor/libogg"
	include "vendor/bit7z"