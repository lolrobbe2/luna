project "imguizmo"
	kind "StaticLib"
	language "C++"
	staticruntime "off"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"../imguizmo/GraphEditor.h",
        "../imguizmo/ImCurveEdit.h",
        "../imguizmo/ImGradient.h",
        "../imguizmo/ImGuizmo.h",
        "../imguizmo/ImSequencer.h",
        "../imguizmo/ImZoomSlider.h",

        "../imguizmo/GraphEditor.cpp",
        "../imguizmo/ImCurveEdit.cpp",
        "../imguizmo/ImGradient.cpp",
        "../imguizmo/ImGuizmo.cpp",
        "../imguizmo/ImSequencer.cpp"
	}
	includedirs
	{
        "../imguizmo"
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