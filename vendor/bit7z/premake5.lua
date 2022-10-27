project "bit7z"
    kind "StaticLib"
    language "C++"
    
	targetdir ("../../bin/" .. outputdir .. "/%{prj.name}")
    objdir ("../../build/" .. outputdir .. "/%{prj.name}")

    includedirs
    {
        "include/",
        "lib/7zSDK/CPP/"
    }
    
    files
    {
        "include/bit7z.hpp",
        "include/bit7zlibrary.hpp",
        "include/bitarchivecreator.hpp",
        "include/bitarchivehandler.hpp",
        "include/bitarchiveinfo.hpp",
        "include/bitarchiveitem.hpp",
        "include/bitarchiveopener.hpp",
        "include/bitcompressionlevel.hpp",
        "include/bitcompressionmethod.hpp",
        "include/bitcompressor.hpp",
        "include/bitexception.hpp",
        "include/bitextractor.hpp",
        "include/bitformat.hpp",
        "include/bitguids.hpp",
        "include/bitinputarchive.hpp",
        "include/bitmemcompressor.hpp",
        "include/bitmemextractor.hpp",
        "include/bitpropvariant.hpp",
        "include/bitstreamcompressor.hpp",
        "include/bitstreamextractor.hpp",
        "include/bittypes.hpp",
        "include/bufferextractcallback.hpp",
        "include/bufferupdatecallback.hpp",
        "include/callback.hpp",
        "include/cbufoutstream.hpp",
        "include/cmultivoloutstream.hpp",
        "include/cstdinstream.hpp",
        "include/cstdoutstream.hpp",
        "include/extractcallback.hpp",
        "include/fileextractcallback.hpp",
        "include/fileupdatecallback.hpp",
        "include/fsindexer.hpp",
        "include/fsitem.hpp",
        "include/fsutil.hpp",
        "include/opencallback.hpp",
        "include/streamextractcallback.hpp",
        "include/streamupdatecallback.hpp",
        "include/updatecallback.hpp",
        "lib/7zSDK/C/Alloc.c",
        "lib/7zSDK/CPP/Windows/FileDir.cpp",
        "lib/7zSDK/CPP/Windows/FileFind.cpp",
        "lib/7zSDK/CPP/Windows/FileIO.cpp",
        "lib/7zSDK/CPP/Windows/FileName.cpp",
        "lib/7zSDK/CPP/7zip/Common/FileStreams.cpp",
        "lib/7zSDK/CPP/Common/IntToString.cpp",
        "lib/7zSDK/CPP/Common/MyString.cpp",
        "lib/7zSDK/CPP/Common/MyVector.cpp",
        "lib/7zSDK/CPP/7zip/Common/StreamObjects.cpp",
        "src/bit7zlibrary.cpp",
        "src/bitarchivecreator.cpp",
        "src/bitarchivehandler.cpp",
        "src/bitarchiveinfo.cpp",
        "src/bitarchiveitem.cpp",
        "src/bitarchiveopener.cpp",
        "src/bitcompressor.cpp",
        "src/bitexception.cpp",
        "src/bitextractor.cpp",
        "src/bitformat.cpp",
        "src/bitguids.cpp",
        "src/bitinputarchive.cpp",
        "src/bitmemcompressor.cpp",
        "src/bitmemextractor.cpp",
        "src/bitpropvariant.cpp",
        "src/bitstreamcompressor.cpp",
        "src/bitstreamextractor.cpp",
        "src/bufferextractcallback.cpp",
        "src/bufferupdatecallback.cpp",
        "src/callback.cpp",
        "src/cbufoutstream.cpp",
        "src/cmultivoloutstream.cpp",
        "src/cstdinstream.cpp",
        "src/cstdoutstream.cpp",
        "src/extractcallback.cpp",
        "src/fileextractcallback.cpp",
        "src/fileupdatecallback.cpp",
        "src/fsindexer.cpp",
        "src/fsitem.cpp",
        "src/fsutil.cpp",
        "src/opencallback.cpp",
        "src/streamextractcallback.cpp",
        "src/streamupdatecallback.cpp",
        "src/updatecallback.cppdialect",
    }
    
    defines
    {
        "WIN32",
        "_WINDOWS",
        "UNICODE",
        "_UNICODE",
        "_7Z_VOL",
        "WIN64",
    }    
	
	links
	{
		"7z.dll"
	}

    filter "configurations:Debug"
        targetsuffix "_d"
        staticruntime "on"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        staticruntime "on"
        runtime "Release"
        optimize "on"
