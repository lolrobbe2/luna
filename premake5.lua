buildmessage ("message")
workspace "luna"
    architecture "x64"
    
    configurations
    {
        "debug",
        "release"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

buildmessage ("$(VULKAN_SDK)/include")
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/luna/thirdParty/GLFW/include"
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
        "%{prj.name}/src/**.cpp"
    }
    includedirs
    {
        "$(VULKAN_SDK)/include",
        "luna/thirdParty/GLFW/include",
        "luna/thirdParty/glm",
        "luna/thirdParty/VMA/include",
        "luna/thirdParty/spdlog/include",
        "luna/src"
    }

    buildoptions
    {
        "/MT"
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
            "GLFW",
            "vulkan-1", 
        }
        filter "configurations:debug"
        symbols "On"
  
        filter "configurations:release"
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
        "luna/src"
    }
   
    links
    {
        
        "luna"
    }
    filter "system:windows"
        cppdialect "c++17"
        staticruntime "on"
        systemversion "latest"
        defines
        {
            "_WINDLL"
        }

buildoptions 
{
    "/MT",
}
