Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
project "VkBootstrap"
	kind "StaticLib"
	language "C++"
	cppdialect "c++20"

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
	filter "configurations:debug"
		symbols "on"

	filter "configurations:release"
		optimize "on"

	filter "configurations:distribution"
		optimize "on"
		symbols "off"