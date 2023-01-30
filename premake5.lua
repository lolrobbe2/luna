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

buildmessage ("$(VULKAN_SDK)/include")
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/luna/thirdParty/GLFW/include"
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
        "luna/thirdParty/GLFW/include",
        "luna/thirdParty/glm",
        "luna/thirdParty/VMA/include",
        "luna/thirdParty/spdlog/include",
        "luna/thirdParty/stb",
        "luna/thirdParty/Vkbootstrap/src",
        "luna/thirdParty/imGui/",
        "luna/src"
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
        "luna/thirdParty/GLFW/include",
        "luna/thirdParty/glm",
        "luna/thirdParty/VMA/include",
        "luna/thirdParty/spdlog/include",
        "luna/thirdParty/stb",
        "luna/thirdParty/Vkbootstrap/src",
        "luna/thirdParty/imGui/",
        "luna/src"
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

 
