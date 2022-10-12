project "OpenAL"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	staticruntime "on"

	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
	objdir ("../../build/" .. outputdir .. "/%{prj.name}")

	includedirs
	{
		"srcAL",
		"srcAL/alc",
		"srcAL/common",
		"includeAL",
		"include/AL"
	}

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	defines
	{
		"AL_LIBTYPE_STATIC"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"WIN32",
			"_WINDOWS",
			"AL_BUILD_LIBRARY",
			"AL_ALEXT_PROTOTYPES",
			"_WIN32",
			"_WIN32_WINNT=0x0502",
			"_CRT_SECURE_NO_WARNINGS",
			"NOMINMAX",
			"CMAKE_INTDIR=\"Debug\"",
			"OpenAL_EXPORTS"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"