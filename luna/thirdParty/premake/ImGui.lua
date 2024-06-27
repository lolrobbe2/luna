project "imGui"
	kind "StaticLib"
	cppdialect "c++20"

	language "C++"
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
	defines
    {
        "IMGUI_API=__declspec(dllexport)"
    }
	filter "configurations:debug"
		symbols "on"

	filter "configurations:release"
		optimize "on"


	filter "configurations:distribution"
		optimize "on"
		symbols "off"
