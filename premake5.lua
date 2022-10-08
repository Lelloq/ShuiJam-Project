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
		defines "SJ_DEBUG"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		defines "SJ_RELEASE"
		runtime "Release"
		optimize "On"

group "Vendor"
	include "vendor/glfw"
	include "vendor/googletest"
	include "vendor/Glad"
