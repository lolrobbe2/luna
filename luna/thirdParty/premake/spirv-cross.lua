Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
project "spirv-cross"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
	
	}

	
	filter "system:windows"
		systemversion "latest"

		files
		{
		}

		defines 
		{ 

			"_CRT_SECURE_NO_WARNINGS"
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
