#pragma once
#pragma warning(push,0)
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
#include <core/platform/platform.h>
/*thirdparty includes*/
#define GLFW_INCLUDE_VULKAN

/*renderer includes*/
#include <VkBootstrap.h>
#include <VkBootstrapDispatch.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <stb_image.h>
#include <stb_image_write.h>
/*scene system includes*/
#include <entt.h>
#pragma warning(pop)


