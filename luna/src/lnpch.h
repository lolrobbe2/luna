#pragma once
#pragma warning(disable : 4005)
#pragma warning(push,0)
#include <core/platform/platform.h>
#include <cstdint>
#include <cstdlib>
#include <string>
#include <functional>
#include <sstream>
#include <map>
#include <optional>
#include <set>
#include <random>
#include <iostream>
#include <fstream>
#include <filesystem>
#include <stddef.h>

#include <shared_mutex>

/*personal engine includes*/

/*thirdparty includes*/
#define GLFW_INCLUDE_VULKAN

/*renderer includes*/
#include <VkBootstrap.h>
#include <VkBootstrapDispatch.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#define GLM_FORCE_SSE2 // or GLM_FORCE_SSE42 if your processor supports it
#define GLM_FORCE_RADIANS
#define GLM_FORCE_INLINE 
#include <glm/glm.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
#pragma warning(default : 4005)
/*scene system includes*/
#include <entt.h>
#pragma warning(pop)


