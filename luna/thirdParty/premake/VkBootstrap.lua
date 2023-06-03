Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
project "VkBootstrap"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	configmap 
	{ }
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"../VkBootstrap/src/VkBootstrap.h",
		"../VkBootstrap/src/VkBootstrapDispatch.h",
		"../VkBootstrap/src/VkBootstrap.cpp",
		
	}
	includedirs
    {
        "$(VULKAN_SDK)/include"
	}
	links
	{
	}
	buildoptions 
	{
		"/MD",
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