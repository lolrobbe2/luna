startproject "test"
workspace "test"
	architecture "x64"
	configurations
	{
		"debug",
		"release",
		"distribution"
	}
project "test"
	location "C:\\Users\\robbe\\source\\repos\\lolrobbe2\\luna\\sandbox\\test"
	outputdir = "% {cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
	LibraryDir = {}
	LibraryDir["scriptCore"] = "%{wks.location}/lib/"
	Library = {}
	Library["scriptCore"] = "%{LibraryDir.scriptCore}/scriptCore.dll"
	kind "SharedLib"
	language "c#"
    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
	objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    links
		{
			"%{Library.scriptCore}"
		}
	filter "configurations:debug"
		optimize "Off"
		symbols "Default"
	filter "configurations:release"
		optimize "On"
		symbols "Default"
	filter "configurations:distribution"
		optimize "Full"
		symbols "Off"
