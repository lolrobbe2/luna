buildmessage ("message")
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

IncludeDir["luna"] = "%{wks.location}/luna/src"

LibraryDir = {}
LibraryDir["VulkanSDK"] = "$(VULKAN_SDK)/Lib"

Library = {}

Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
Library["SPIRV_Tools"] = "%{LibraryDir.VulkanSDK}/SPIRV-Tools.lib"


include "luna/thirdParty/"
project "luna"
    location "luna"
    kind "SharedLib"
    language "c++"
    
    targetdir("%{wks.location}/bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/x64/%{prj.name}")
    files
    {
        "%{prj.name}/src/**.h",
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
        ("{copy} %{cfg.buildtarget.relpath} %{wks.location}/bin/" .. outputdir .. "/x64/sandbox")
    }
    filter "system:windows"
    
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        symbols "on"
        defines
        {
            "_CRT_SECURE_NO_WARNINGS",
            "LN_BUILD_DLL",
            "_WINDLL"
           
        }
        links
        {
            "%{Library.ShaderC}",
			"%{Library.SPIRV_Cross}",
			"%{Library.SPIRV_Cross_GLSL}",
            "GLFW",
            "VkBootstrap",
            "imGui",
            "stb",
            "vulkan-1"
        }
        filter "configurations:debug"
       
        symbols "On"
  
        filter "configurations:release"

        optimize "On"

        filter "configurations:distribution"
       
        symbols "Off"
        optimize "On"



 
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
        "$(VULKAN_SDK)/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spd}",
        "%{IncludeDir.vma}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.vkb}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.luna}"
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
       
        symbols "On"
  
        filter "configurations:release"
       
        optimize "On"

        filter "configurations:distribution"
       
        symbols "Off"
        optimize "On"

project "apollo"
    location "apollo"
    kind "WindowedApp"
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
        "$(VULKAN_SDK)/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spd}",
        "%{IncludeDir.vma}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.vkb}",
        "%{IncludeDir.imgui}",
        "%{IncludeDir.luna}"
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
    
        symbols "On"

        filter "configurations:release"
    
        optimize "On"

        filter "configurations:distribution"
    
        symbols "Off"
        optimize "On"
