buildmessage ("message")
flags
{
    "MultiProcessorCompile"
}



startproject "sandbox"
workspace "luna"
    architecture "x64"
    
    configurations
    {
        "debug",
        "release",
        "distribution"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

buildmessage("$(VULKAN_SDK)/include")
IncludeDir = {}
IncludeDir["GLFW"] =  "%{wks.location}/luna/thirdParty/GLFW/include"
IncludeDir["entt"] =  "%{wks.location}/luna/thirdParty/entt"
IncludeDir["glm"] =   "%{wks.location}/luna/thirdParty/glm"
IncludeDir["vma"] =   "%{wks.location}/luna/thirdParty/VMA/include"
IncludeDir["vkb"] =   "%{wks.location}/luna/thirdParty/Vkbootstrap/src"
IncludeDir["stb"] =   "%{wks.location}/luna/thirdParty/stb"
IncludeDir["spd"] =   "%{wks.location}/luna/thirdParty/spdlog/include"
IncludeDir["imgui"] = "%{wks.location}/luna/thirdParty/imGui/"
IncludeDir["imguizmo"] = "%{wks.location}/luna/thirdParty/imguizmo"
IncludeDir["yaml_cpp"] = "%{wks.location}/luna/thirdParty/yaml-cpp/include"
IncludeDir["mono"] =  "%{wks.location}/luna/thirdParty/mono/include"

IncludeDir["luna"] = "%{wks.location}/luna/src"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "$(VULKAN_SDK)/Lib"

Library = {}

Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC"] = "%{wks.location}/vendor/shaderc_shared.lib"
Library["SPIRV_Cross"] = "%{wks.location}/vendor/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL"] = "%{wks.location}/vendor/spirv-cross-glsl.lib"
Library["SPIRV_Tools"] = "%{wks.location}/vendor/SPIRV-Tools.lib"



include "luna/thirdParty/"
group"core"
project "luna"
    location "luna"
    kind "SharedLib"
    language "c++"
    toolset "v142"
    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.cpp",
       
    }
    includedirs
    {
        "$(VULKAN_SDK)/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spd}",
        "%{IncludeDir.vma}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.vkb}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.imguizmo}",
        "%{IncludeDir.yaml_cpp}",
        "%{IncludeDir.mono}",
        "%{IncludeDir.luna}"
        
    }

    buildoptions
    {
        "/MD"
    }
    
    libdirs
    {
        "$(VULKAN_SDK)/Lib", 
    }
    postbuildcommands
    {
        ("{copy} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/x64/sandbox"),
        ("{copy} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/x64/apollo")
    }
    filter "system:windows"
    
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        symbols "on"

        links
        {
            "%{Library.ShaderC}",
			"%{Library.SPIRV_Cross}",
			"%{Library.SPIRV_Cross_GLSL}",
            "GLFW",
            "VkBootstrap",
            "imGui",
            "stb",
            "yaml-cpp",
            "vulkan-1",
            "Ws2_32",
            "Mswsock.lib",
            "AdvApi32.lib"
        }
        filter "configurations:debug"
            LibraryDir["mono"] = "%{wks.location}/luna/thirdParty/mono/lib/debug/"
            Library["mono"] = "%{LibraryDir.mono}/libmono-static-sgen.lib"
            links
            {
                "%{Library.mono}"
            }
            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "LN_BUILD_DLL",
                "_WINDLL",
                "LN_DEBUG"
            
            }
            runtime "Debug"
            symbols "On"
  
        filter "configurations:release"
            LibraryDir["mono"] = "%{wks.location}/luna/thirdParty/mono/lib/release/"
            Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"
            links
            {
                "%{Library.mono}"
            }
            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "LN_BUILD_DLL",
                "_WINDLL",
                "LN_RELEASE"
            
            }
            runtime "Release"
            optimize "On"

        filter "configurations:distribution"
            LibraryDir["mono"] = "%{wks.location}/luna/thirdParty/mono/lib/release/"
            Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"
            links
            {
                "%{Library.mono}"
            }
            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "LN_BUILD_DLL",
                "_WINDLL",
                "LN_DISTRIBUTION"
            
            }
            runtime "Release"
            symbols "Off"
            optimize "On"
            buildoptions 
            {
                "-mwindows"
            }
        filter { "action:vs*" } -- Visual Studio-specific configuration
            buildoptions { "/w" } -- Suppress all warnings
group""
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "c++"
    toolset "v142"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    includedirs
    {
        "$(VULKAN_SDK)/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spd}",
        "%{IncludeDir.vma}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.vkb}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.luna}",
        "%{prj.name}/src"
    }
   
    links
    {
        
        "luna"
    }
    buildoptions 
    {
        "/MD",
    }
    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        defines
        {
            "_WINDLL"
        }
        
        filter "configurations:debug"
            runtime "Debug"
            symbols "On"
  
        filter "configurations:release"
            runtime "Release"
            optimize "On"

        filter "configurations:distribution"
            runtime "Release"
            symbols "Off"
            optimize "On"
        buildoptions 
        {
            "-mwindows"
        }
group"core"
project "apollo"
    location "apollo"
    kind "ConsoleApp"
    language "c++"
    toolset "v142"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    includedirs
    {
        "$(VULKAN_SDK)/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spd}",
        "%{IncludeDir.vma}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.vkb}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.luna}",
        "%{prj.name}/src"
    }

    links
    {
  
        "luna"
    }
    buildoptions 
    {
        "/MD",
    }
    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        defines
        {
            "_WINDLL",
        }
        
        filter "configurations:debug"
        runtime "Debug"
        symbols "On"

        filter "configurations:release"
        runtime "Release"
        optimize "On"

        filter "configurations:distribution"
        runtime "Release"
        symbols "Off"
        optimize "On"
        buildoptions 
        {
            "-mwindows"
        }
group""



workspace "luna scripting"
    architecture "x64"
    
    configurations
    {
        "debug",
        "release",
        "distribution"
    }

project "sharpSandbox"
    location "sharpSandbox"
    kind "SharedLib"
    language "c#"
    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.cs"
    }
    links
    {
        "scriptCore"
    }
    postbuildcommands
    {
        ("{copy} %{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name} %{wks.location}apollo/mono/lib")
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
project "scriptCore"
    location "scriptCore"
    kind "SharedLib"
    language "c#"
    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    documentationFile ""
    files
    {
        "%{prj.name}/src/**.cs"
    }
        postbuildcommands
    {
        ("{copy} %{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name} %{wks.location}apollo/mono/lib")
    }

    links
    {
        "mono-2.0"
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
            