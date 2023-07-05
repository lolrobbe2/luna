project "zlib"
	kind "StaticLib"
	language "C++"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
	"../zlib/adler32.c",
	"../zlib/compress.c",
	"../zlib/crc32.c",
	"../zlib/deflate.c",
	"../zlib/infback.c",
	"../zlib/inffast.c",
	"../zlib/inflate.c",
	"../zlib/inftrees.c",
	"../zlib/trees.c",
	"../zlib/uncompr.c",
	"../zlib/zutil.c",
	}
    buildoptions
    {
        "/MD"
    }
	includedirs
	{
		"../zlib/"
	}
	defines
	{
		
	}
	filter "system:windows"
		systemversion "latest"
		staticruntime "off"

		filter "configurations:debug"
			runtime "Debug"
			symbols "on"

		filter "configurations:release"
			runtime "Release"
			optimize "on"

		filter "configurations:distribution"
			runtime "Release"
			optimize "on"



	