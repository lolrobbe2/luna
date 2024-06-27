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

buildmessage("$(VULKAN_SDK)/Include")
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

Library["ShaderC"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
Library["SPIRV_Tools"] = "%{LibraryDir.VulkanSDK}/SPIRV-Tools.lib"

Library["ShaderC_d"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_d"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_d"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_d"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"
--[[

Library["ShaderC"] = "$(VULKAN_SDK)/bin/shaderc_shared.dll"
Library["SPIRV_Cross"] = "$(VULKAN_SDK)/bin/spirv-cross-c-shared.dll"
Library["SPIRV_Tools"] = "$(VULKAN_SDK)/bin/SPIRV-Tools-shared.dll"

Library["ShaderC_d"] = "$(VULKAN_SDK)/bin/shaderc_sharedd.dll"
Library["SPIRV_Cross_d"] = "$(VULKAN_SDK)/bin/spirv-cross-c-sharedd.dll"
Library["SPIRV_Tools_d"] = "$(VULKAN_SDK)/bin/SPIRV-Tools-sharedd.dll"
]]--

include "luna/thirdParty/"
group"core"
project "luna"
    location "luna"
    kind "SharedLib"
    language "c++"
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
        "$(VULKAN_SDK)/Include",
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
    
    libdirs
    {
        "$(VULKAN_SDK)/Lib", 
    }
    postbuildcommands
    {
        ("@python %{wks.location}copyEngineCore.py")
    }
    filter "system:windows"
        cppdialect "c++20"
        systemversion "latest"
        symbols "on"
        links
        {

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
                "%{Library.mono}",
                "%{Library.ShaderC}",
			    "%{Library.SPIRV_Cross_d}",
                "%{Library.SPIRV_Cross_GLSL_d}",
                "%{Library.SPIRV_Tools_d}"
            }
            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "LN_BUILD_DLL",
                "LN_DEBUG"
            
            }
            symbols "On"
  
        filter "configurations:release"
            LibraryDir["mono"] = "%{wks.location}/luna/thirdParty/mono/lib/release/"
            Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"
            links
            {
                "%{Library.mono}",
                "%{Library.ShaderC}",
			    "%{Library.SPIRV_Cross}",
                "%{Library.SPIRV_Cross_GLSL}",
                "%{Library.SPIRV_Tools}"
            }
            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "LN_BUILD_DLL",
                "LN_RELEASE"
            
            }
            optimize "On"

        filter "configurations:distribution"
            LibraryDir["mono"] = "%{wks.location}/luna/thirdParty/mono/lib/release/"
            Library["mono"] = "%{LibraryDir.mono}/mono-2.0-sgen.lib"
            links
            {
                "%{Library.mono}",
                "%{Library.ShaderC}",
			    "%{Library.SPIRV_Cross}",
                "%{Library.SPIRV_Cross_GLSL}",
                "%{Library.SPIRV_Tools}"
            }
            defines
            {
                "_CRT_SECURE_NO_WARNINGS",
                "LN_BUILD_DLL",
                "LN_DISTRIBUTION"
            
            }
            symbols "Off"
            optimize "On"
group""
project "sandbox"
    location "sandbox"
    kind "ConsoleApp"
    language "c++"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    includedirs
    {
        "$(VULKAN_SDK)/Include",
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

    filter "system:windows"
        cppdialect "c++17"
        systemversion "latest"
        filter "configurations:debug"
            defines
            {
                "IMGUI_DISABLE"
            }
        
            symbols "On"
        filter "configurations:release"
            defines
            {
                "IMGUI_DISABLE"
            }
        
            optimize "On"
        filter "configurations:distribution"
            defines
            {
                "IMGUI_DISABLE"
            }
        
            symbols "Off"
            optimize "On"
group"core"
project "apollo"
    location "apollo"
    kind "ConsoleApp"
    language "c++"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    includedirs
    {
        "$(VULKAN_SDK)/Include",
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

    filter "system:windows"
        cppdialect "c++17"
        systemversion "latest"
        defines
        {
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
group""



workspace "lunaScripting"
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
    postbuildcommands
    {
        ("@python %{wks.location}copyScriptCore.py")
    }
    files
    {
        "%{prj.name}/src/**.cs"
    }
    links
    {
        "scriptCore"
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
        ("@python %{wks.location}copyScriptCore.py")
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
            