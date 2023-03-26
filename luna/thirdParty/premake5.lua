Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
group "thirdParty"

include "premake/GLFW.lua"
include "premake/VkBootstrap.lua"
include "premake/ImGui.lua"
include "premake/stb.lua"
include "premake/yaml-cpp"
include "premake/mono.lua"

group ""