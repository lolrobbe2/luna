project "stb"
	kind "StaticLib"
	language "C++"
	files
	{
		"../stb.c",
		"../stb/stb_include.h",
		"../stb/stb_image.h",
		"../stb/stb_truetype.h",
		"../stb/stb_image_write.h"
	}
	buildoptions 
	{
	}
	includedirs
	{
		"../stb"
	}

	filter "system:windows"
		cppdialect "c++20"
		systemversion "latest"
		defines
		{

		}

	filter "configurations:debug"
		symbols "on"

	filter "configurations:release"
		optimize "on"

	filter "configurations:distribution"
		optimize "on"
		symbols "off"