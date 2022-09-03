buildmessage ("message")
workspace "luna"
    architecture "x64"
    
    configurations
    {
        "debug",
        "release"
    }
    
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

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
        "thirdParty/glfw-3.3.6.bin.WIN64/include",
        "thirdParty/glm-master/glm",
        "thirdParty/",
        "thirdParty/stb-master",
        "thirdParty/VulkanMemoryAllocator-master/include",
        "luna/src"
    }

    links
    {
        "glfw3_mt.lib",
        "$(VULKAN_SDK)/lib/vulkan-1.lib"
    }
    
    libdirs
    {
        "thirdParty/glfw-3.3.7.bin.WIN64/lib-vc2022", 
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
        postbuildcommands
        {
            ("{copy} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/x64/sandbox")
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
        "thirdParty/glfw-3.3.7.bin.WIN64/include",
        "thirdParty/glm-master/glm",
        "thirdParty/",
        "thirdParty/stb-master",
        "thirdParty/VulkanMemoryAllocator-master/include",
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
