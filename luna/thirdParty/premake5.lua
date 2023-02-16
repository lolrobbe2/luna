Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
group "thirdParty"
project "GLFW"
	kind "StaticLib"
	language "C"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"GLFW/include/GLFW/glfw3.h",
		"GLFW/include/GLFW/glfw3native.h",
		"GLFW/src/glfw_config.h",
		"GLFW/src/context.c",
		"GLFW/src/init.c",
		"GLFW/src/input.c",
		"GLFW/src/monitor.c",

		"GLFW/src/null_init.c",
		"GLFW/src/null_joystick.c",
		"GLFW/src/null_monitor.c",
		"GLFW/src/null_window.c",

		"GLFW/src/platform.c",
		"GLFW/src/vulkan.c",
		"GLFW/src/window.c",
	}

	filter "system:linux"
		pic "On"

		systemversion "latest"
		
		files
		{
			"GLFW/src/x11_init.c",
			"GLFW/src/x11_monitor.c",
			"GLFW/src/x11_window.c",
			"GLFW/src/xkb_unicode.c",
			"GLFW/src/posix_time.c",
			"GLFW/src/posix_thread.c",
			"GLFW/src/glx_context.c",
			"GLFW/src/egl_context.c",
			"GLFW/src/osmesa_context.c",
			"GLFW/src/linux_joystick.c"
		}

		defines
		{
			"_GLFW_X11"
		}

	filter "system:windows"
		systemversion "latest"

		files
		{
			"GLFW/src/win32_init.c",
			"GLFW/src/win32_joystick.c",
			"GLFW/src/win32_module.c",
			"GLFW/src/win32_monitor.c",
			"GLFW/src/win32_time.c",
			"GLFW/src/win32_thread.c",
			"GLFW/src/win32_window.c",
			"GLFW/src/wgl_context.c",
			"GLFW/src/egl_context.c",
			"GLFW/src/osmesa_context.c"
		}

		defines 
		{ 
			"_GLFW_WIN32",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links
		{
			"msvcrt",
			"Dwmapi.lib"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
        symbols "off"


project "VkBootstrap"
	kind "StaticLib"
	language "C++"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"VkBootstrap/src/VkBootstrap.h",
		"VkBootstrap/src/VkBootstrapDispatch.h",
		"VkBootstrap/src/VkBootstrap.cpp",
		
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
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"

project "imGui"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	files
	{
		"imGui/imConfig.h",
		"imGui/imgui.h",
		"imGui/imgui.cpp",

		"imGui/imgui_demo.cpp",
		"imGui/imgui_draw.cpp",
		"imGui/imgui_widgets.cpp",
		"imGui/imgui_tables.cpp",
		"imGui/imgui_tables.cpp",

		"imGui/backends/imgui_impl_vulkan.cpp",
		"imGui/backends/imgui_impl_glfw.cpp"
	}
	includedirs
	{
		"$(VULKAN_SDK)/include",
		"imgui",
		"GLFW/include"
	}
	buildoptions 
	{
		"/MD",
	}
	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"

project "stb"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	files
	{
		"stb.c",
		"stb/stb_include.h",
		"stb/stb_image.h"
	}
	buildoptions 
	{
		"/MD",
	}
	includedirs
	{
		"stb"
	}

	filter "system:windows"
		cppdialect "c++17"
		staticruntime "on"
		systemversion "latest"
		defines
		{
			"STB_IMAGE_IMPLEMENTATION"
		}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		runtime "Release"
		optimize "on"
		symbols "off"
group ""