#pragma once
#include <core/artemis/device/allocator.h>
#include <core/artemis/rendering/renderPass.h>
#include <core/artemis/rendering/builders/pipelineBuilder.h>
#include <core/artemis/device/builders/descriptorPoolBuilder.h>
#include <core/artemis/rendering/sampler.h>
namespace luna 
{
	namespace artemis 
	{
        enum physicalDeviceFeatures {
            ROBUST_BUFFER_ACCESS,
            FULL_DRAW_INDEX_UINT32,
            IMAGE_CUBE_ARRAY,
            INDEPENDENT_BLEND,
            GEOMETRY_SHADER,
            TESSELLATION_SHADER,
            SAMPLE_RATE_SHADING,
            DUAL_SRC_BLEND,
            LOGIC_OP,
            MULTI_DRAW_INDIRECT,
            DRAW_INDIRECT_FIRST_INSTANCE,
            DEPTH_CLAMP,
            DEPTH_BIAS_CLAMP,
            FILL_MODE_NON_SOLID,
            DEPTH_BOUNDS,
            WIDE_LINES,
            LARGE_POINTS,
            ALPHA_TO_ONE,
            MULTI_VIEWPORT,
            SAMPLER_ANISOTROPY,
            TEXTURE_COMPRESSION_ETC2,
            TEXTURE_COMPRESSION_ASTC_LDR,
            TEXTURE_COMPRESSION_BC,
            OCCLUSION_QUERY_PRECISE,
            PIPELINE_STATISTICS_QUERY,
            VERTEX_PIPELINE_STORES_AND_ATOMICS,
            FRAGMENT_STORES_AND_ATOMICS,
            SHADER_TESSELLATION_AND_GEOMETRY_POINT_SIZE,
            SHADER_IMAGE_GATHER_EXTENDED,
            SHADER_STORAGE_IMAGE_EXTENDED_FORMATS,
            SHADER_STORAGE_IMAGE_MULTISAMPLE,
            SHADER_STORAGE_IMAGE_READ_WITHOUT_FORMAT,
            SHADER_STORAGE_IMAGE_WRITE_WITHOUT_FORMAT,
            SHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING,
            SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING,
            SHADER_STORAGE_BUFFER_ARRAY_DYNAMIC_INDEXING,
            SHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING,
            SHADER_CLIP_DISTANCE,
            SHADER_CULL_DISTANCE,
            SHADER_FLOAT64,
            SHADER_INT64,
            SHADER_INT16,
            SHADER_RESOURCE_RESIDENCY,
            SHADER_RESOURCE_MIN_LOD,
            SPARSE_BINDING,
            SPARSE_RESIDENCY_BUFFER,
            SPARSE_RESIDENCY_IMAGE_2D,
            SPARSE_RESIDENCY_IMAGE_3D,
            SPARSE_RESIDENCY_2_SAMPLES,
            SPARSE_RESIDENCY_4_SAMPLES,
            SPARSE_RESIDENCY_8_SAMPLES,
            SPARSE_RESIDENCY_16_SAMPLES,
            SPARSE_RESIDENCY_ALIASED,
            VARIABLE_MULTISAMPLE_RATE,
            INHERITED_QUERIES,
        };

		class device
		{
		public:
            device() = default;
			device(const ref<vulkan::window>& Window);
            ~device();
            bool isFeatureSupported(physicalDeviceFeatures feature) const;
            VkSurfaceCapabilitiesKHR surfaceCapabilities() const;

            VkQueue getQueue(vkb::QueueType type,bool dedicated = false) const;
            uint32_t getQueueFamilyIndex(vkb::QueueType type, bool dedicated = false) const;
            bool hasDedicatedQueue(vkb::QueueType type) const;
            ref<commandPool> getCommandPool(vkb::QueueType type, const VkCommandPoolCreateFlags createFlags = 0) const;
            ref<semaphore> getSemaphore(const VkSemaphoreCreateFlags flags) const;
            ref<fence> getFence(const VkFenceCreateFlags flags) const;
            ref<swapchain> getSwapchain() const;
            ref<sampler> getSampler(const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode = VK_SAMPLER_ADDRESS_MODE_REPEAT) const;
            ref<allocator> getAllocator(const ref<commandPool> transferPool = nullptr) const;
            descriptorPoolBuilder getDescriptorPoolBuilder(const ref<shader> shader) const;
            pipelineBuilder getPipelineBuilder() const;
            renderPassBuilder getRenderPassBuilder() const;
            void waitIdle() const;
            operator const VkDevice* () const { return &_device.device; }
            operator const VkDevice () const { return _device.device; }
            operator const device* () const { return this; }
            operator GLFWwindow* () const { return (GLFWwindow*)window->getWindow(); }
            operator VkInstance () const { return instance; }
            operator VkPhysicalDevice () const { return physicalDevice; }
		private:
			VkResult createInstance();

			std::vector<const char*> getRequiredExtensions();
			VkResult pickPhysicalDevice();
            VkResult createLogicalDevice();
			/**
			 * @brief the debug callback function for vulkan debug and error catching.
			 *
			 * \param messageSeverity
			 * \param messageTypes
			 * \param pCallbackData
			 * \param pUserData
			 * \return
			 */
			static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
				VkDebugUtilsMessageTypeFlagsEXT messageTypes,
				const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
				void* pUserData);
			ref<vulkan::window> window;

			VkApplicationInfo appInfo{};
			vkb::Instance instance;
			vkb::Device _device;
			vkb::PhysicalDevice physicalDevice;


			VkSurfaceKHR surface = VK_NULL_HANDLE; // render surface
			VkPhysicalDeviceFeatures supportedFeatures{};
		};
	}
}


