project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	cppdialect "c++20"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"../yaml-cpp/src/**.h",
		"../yaml-cpp/src/**.cpp",
		
		"../yaml-cpp/include/**.h"
	}

	includedirs
	{
		"../yaml-cpp/include"
	}
	defines
	{
		"YAML_CPP_STATIC_DEFINE"
	}
	filter "system:windows"
		systemversion "latest"

		filter "configurations:debug"

			symbols "on"

		filter "configurations:release"

			optimize "on"

		filter "configurations:distribution"

			optimize "on"



	