project "stb"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	files
	{
		"../stb.c",
		"../stb/stb_include.h",
		"../stb/stb_image.h",
		"../stb/stb_truetype.h"
	}
	buildoptions 
	{
		"/MD",
	}
	includedirs
	{
		"../stb"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "on"
		systemversion "latest"
		defines
		{
			"STB_IMAGE_IMPLEMENTATION"
		}

	filter "configurations:debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:release"
		runtime "Release"
		optimize "on"

	filter "configurations:distribution"
		runtime "Release"
		optimize "on"
		symbols "off"