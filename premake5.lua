buildmessage ("message")
workspace "luna"
    architecture "x64"
    
    configurations
    {
        "debug",
        "release"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


IncludeDir = {}
IncludeDir["GLFW"] = "luna/thirdParty/GLFW/include"
include "luna/thirdParty/GLFW"
project "luna"
    location "luna"
    kind "SharedLib"
    language "c++"
    
    targetdir("bin/" .. outputdir .. "/x64/%{prj.name}")
    objdir("bin-int/" .. outputdir .. "/x64/%{prj.name}")
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
        "luna/thirdParty",
        "luna/thirdParty/VMA/include",
        "luna/thirdParty/spdlog/include",
        "luna/src"
    }

    links
    {
        "GLFW",
        "vulkan-1"
        
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
            "LN_BUILD_DLL",
            "_WINDLL"
           
        }
        
        filter "configurations:debug"
        symbols "On"
  
        filter "configurations:release"
        optimize "On"
        buildoptions 
        {
            "/MT",
        }

 
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
        "luna/thirdParty",
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
