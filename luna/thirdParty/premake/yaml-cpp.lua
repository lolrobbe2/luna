project "yaml-cpp"
	kind "StaticLib"
	language "C++"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"../yaml-cpp/src/**.h",
		"../yaml-cpp/src/**.cpp",
		
		"../yaml-cpp/include/**.h"
	}
    buildoptions
    {
        "/MD"
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



	