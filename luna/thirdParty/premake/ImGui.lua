project "imGui"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"../imgui/imConfig.h",
		"../imgui/imgui.h",
		"../imgui/imgui.cpp",

		"../imgui/imgui_demo.cpp",
		"../imgui/imgui_draw.cpp",
		"../imgui/imgui_widgets.cpp",
		"../imgui/imgui_tables.cpp",
		"../imgui/imgui_tables.cpp",

		"../imgui/backends/imgui_impl_vulkan.cpp",
		"../imgui/backends/imgui_impl_glfw.cpp"
	}
	includedirs
	{
		"$(VULKAN_SDK)/include",
		"../imgui",
		"../GLFW/include"
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