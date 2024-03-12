#include "device.h"
#include <core/debug/debugMacros.h>
#include <core/utils/shaderLibrary.h>
namespace luna
{
    namespace artemis
    {
        device::device(const ref<vulkan::window>& Window)
        {
            createInstance();
            VkResult result = glfwCreateWindowSurface(instance, (GLFWwindow*)Window->getWindow(), nullptr, &surface);
            LN_ERR_FAIL_COND_MSG(result != VK_SUCCESS, "[Artemis] an error occured during surface creation, result: " + std::to_string(result));
            pickPhysicalDevice();
            createLogicalDevice();
            window = Window;
            shaderLibrary::init(&_device.device);
        }

        device::~device()
        {
            shaderLibrary::shutdown();
            vkDeviceWaitIdle(_device);
            vkb::destroy_device(_device);
            vkb::destroy_instance(instance);
            _device.device = VK_NULL_HANDLE;
            _device.physical_device.physical_device = VK_NULL_HANDLE;
            instance.instance = VK_NULL_HANDLE;
        }

        ///instance creation
        VkResult device::createInstance()
        {
            vkb::InstanceBuilder instanceBuilder;
            instanceBuilder.set_app_name("luna")
                .set_engine_name("Artemis")
                .set_engine_version(MAJOR, MINOR, PATCH)
                .use_default_debug_messenger()
                .set_debug_callback(debugCallback)
                .require_api_version(1, 2, 0);
#ifdef LN_DEBUG
            instanceBuilder.request_validation_layers(true);
#else
            instanceBuilder.request_validation_layers(false);
#endif // LN_DEBUG

            for (const auto& extension : getRequiredExtensions())
            {
                instanceBuilder.enable_extension(extension);
            }
            auto result = instanceBuilder.build();

            LN_ERR_FAIL_COND_V_MSG(!result, result.vk_result(), "[Artemis] and error ocured during instance creation, msg:" + result.full_error().type.message());

            instance = result.value();
            appInfo.apiVersion = VKB_VK_API_VERSION_1_2;
            return VK_SUCCESS;
        }

        std::vector<const char*> device::getRequiredExtensions()
        {
            uint32_t glfwExtensionCount = 0;
            const char** glfwExtensions;
            glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

            std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

            return extensions;
        }

        //physical device/gpu 
        VkResult device::pickPhysicalDevice()
        {
            vkb::PhysicalDeviceSelector deviceSelector{ instance };
            VkPhysicalDeviceFeatures features{};
            VkPhysicalDeviceFeatures2 features2{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2 };
            VkPhysicalDeviceVulkan12Features features12{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };

            deviceSelector
                .set_minimum_version(1, 2)
                .set_surface(surface);
            auto tempPhysicalDevice = deviceSelector.select().value();
            if (tempPhysicalDevice.physical_device != VK_NULL_HANDLE)
            {
                
                vkGetPhysicalDeviceFeatures(tempPhysicalDevice, &supportedFeatures);

                VkPhysicalDeviceRobustness2FeaturesEXT robustnessFeatures = {};
                robustnessFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT;

                VkPhysicalDeviceFeatures2 features2 = {};
                features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
                features2.pNext = &robustnessFeatures;

                vkGetPhysicalDeviceFeatures2(tempPhysicalDevice, &features2);
                LN_ERR_FAIL_COND_V_MSG(!(robustnessFeatures.nullDescriptor == VK_TRUE), VkResult::VK_ERROR_FEATURE_NOT_PRESENT, "[Artemis] feature not supported: ROBUST_BUFFER_ACCESS");
                LN_ERR_FAIL_COND_V_MSG(!isFeatureSupported(SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING), VkResult::VK_ERROR_FEATURE_NOT_PRESENT, "[Artemis] feature not supported: SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING");
                LN_ERR_FAIL_COND_V_MSG(!isFeatureSupported(MULTI_VIEWPORT), VkResult::VK_ERROR_FEATURE_NOT_PRESENT, "[Artemis] feature not present: MULTI_VIEWPORT");
                features.multiViewport = VK_TRUE;
                features12.shaderSampledImageArrayNonUniformIndexing = VK_TRUE; //shaderStorageImageArrayDynamicIndexing : specifies whether arrays of storage images can be indexed by dynamically uniform integer expressions in shader code
               
                VkPhysicalDeviceRobustness2FeaturesEXT robustnessFeaturesRequest{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ROBUSTNESS_2_FEATURES_EXT };
                robustnessFeaturesRequest.nullDescriptor = VK_TRUE;

                auto result = deviceSelector
                    .set_required_features(features)
                    .set_required_features_12(features12)
                    .add_desired_extension(VK_EXT_ROBUSTNESS_2_EXTENSION_NAME)
                    .add_required_extension_features<VkPhysicalDeviceRobustness2FeaturesEXT>(robustnessFeaturesRequest)
                    .select();
                LN_ERR_FAIL_COND_V_MSG(!result, result.vk_result(), result.error().message());

                physicalDevice = result.value();

                return VK_SUCCESS;
            }
            return VK_ERROR_DEVICE_LOST;
        }

        VkResult device::createLogicalDevice()
        {
            vkb::DeviceBuilder deviceBuilder{ physicalDevice };
            auto result = deviceBuilder.build();
            LN_ERR_FAIL_COND_V_MSG(!result, result.vk_result(), "[Artemis] could not create logical device: " + result.error().message());
            _device = result.value();
            return VK_SUCCESS;
        }

        bool device::isFeatureSupported(physicalDeviceFeatures feature) 
        {
            switch (feature) {
            case ROBUST_BUFFER_ACCESS:
                return (supportedFeatures.robustBufferAccess == VK_TRUE);
            case FULL_DRAW_INDEX_UINT32:
                return (supportedFeatures.fullDrawIndexUint32 == VK_TRUE);
            case IMAGE_CUBE_ARRAY:
                return (supportedFeatures.imageCubeArray == VK_TRUE);
            case INDEPENDENT_BLEND:
                return (supportedFeatures.independentBlend == VK_TRUE);
            case GEOMETRY_SHADER:
                return (supportedFeatures.geometryShader == VK_TRUE);
            case TESSELLATION_SHADER:
                return (supportedFeatures.tessellationShader == VK_TRUE);
            case SAMPLE_RATE_SHADING:
                return (supportedFeatures.sampleRateShading == VK_TRUE);
            case DUAL_SRC_BLEND:
                return (supportedFeatures.dualSrcBlend == VK_TRUE);
            case LOGIC_OP:
                return (supportedFeatures.logicOp == VK_TRUE);
            case MULTI_DRAW_INDIRECT:
                return (supportedFeatures.multiDrawIndirect == VK_TRUE);
            case DRAW_INDIRECT_FIRST_INSTANCE:
                return (supportedFeatures.drawIndirectFirstInstance == VK_TRUE);
            case DEPTH_CLAMP:
                return (supportedFeatures.depthClamp == VK_TRUE);
            case DEPTH_BIAS_CLAMP:
                return (supportedFeatures.depthBiasClamp == VK_TRUE);
            case FILL_MODE_NON_SOLID:
                return (supportedFeatures.fillModeNonSolid == VK_TRUE);
            case DEPTH_BOUNDS:
                return (supportedFeatures.depthBounds == VK_TRUE);
            case WIDE_LINES:
                return (supportedFeatures.wideLines == VK_TRUE);
            case LARGE_POINTS:
                return (supportedFeatures.largePoints == VK_TRUE);
            case ALPHA_TO_ONE:
                return (supportedFeatures.alphaToOne == VK_TRUE);
            case MULTI_VIEWPORT:
                return (supportedFeatures.multiViewport == VK_TRUE);
            case SAMPLER_ANISOTROPY:
                return (supportedFeatures.samplerAnisotropy == VK_TRUE);
            case TEXTURE_COMPRESSION_ETC2:
                return (supportedFeatures.textureCompressionETC2 == VK_TRUE);
            case TEXTURE_COMPRESSION_ASTC_LDR:
                return (supportedFeatures.textureCompressionASTC_LDR == VK_TRUE);
            case TEXTURE_COMPRESSION_BC:
                return (supportedFeatures.textureCompressionBC == VK_TRUE);
            case OCCLUSION_QUERY_PRECISE:
                return (supportedFeatures.occlusionQueryPrecise == VK_TRUE);
            case PIPELINE_STATISTICS_QUERY:
                return (supportedFeatures.pipelineStatisticsQuery == VK_TRUE);
            case VERTEX_PIPELINE_STORES_AND_ATOMICS:
                return (supportedFeatures.vertexPipelineStoresAndAtomics == VK_TRUE);
            case FRAGMENT_STORES_AND_ATOMICS:
                return (supportedFeatures.fragmentStoresAndAtomics == VK_TRUE);
            case SHADER_TESSELLATION_AND_GEOMETRY_POINT_SIZE:
                return (supportedFeatures.shaderTessellationAndGeometryPointSize == VK_TRUE);
            case SHADER_IMAGE_GATHER_EXTENDED:
                return (supportedFeatures.shaderImageGatherExtended == VK_TRUE);
            case SHADER_STORAGE_IMAGE_EXTENDED_FORMATS:
                return (supportedFeatures.shaderStorageImageExtendedFormats == VK_TRUE);
            case SHADER_STORAGE_IMAGE_MULTISAMPLE:
                return (supportedFeatures.shaderStorageImageMultisample == VK_TRUE);
            case SHADER_STORAGE_IMAGE_READ_WITHOUT_FORMAT:
                return (supportedFeatures.shaderStorageImageReadWithoutFormat == VK_TRUE);
            case SHADER_STORAGE_IMAGE_WRITE_WITHOUT_FORMAT:
                return (supportedFeatures.shaderStorageImageWriteWithoutFormat == VK_TRUE);
            case SHADER_UNIFORM_BUFFER_ARRAY_DYNAMIC_INDEXING:
                return (supportedFeatures.shaderUniformBufferArrayDynamicIndexing == VK_TRUE);
            case SHADER_SAMPLED_IMAGE_ARRAY_DYNAMIC_INDEXING:
                return (supportedFeatures.shaderSampledImageArrayDynamicIndexing == VK_TRUE);
            case SHADER_STORAGE_BUFFER_ARRAY_DYNAMIC_INDEXING:
                return (supportedFeatures.shaderStorageBufferArrayDynamicIndexing == VK_TRUE);
            case SHADER_STORAGE_IMAGE_ARRAY_DYNAMIC_INDEXING:
                return (supportedFeatures.shaderStorageImageArrayDynamicIndexing == VK_TRUE);
            case SHADER_CLIP_DISTANCE:
                return (supportedFeatures.shaderClipDistance == VK_TRUE);
            case SHADER_CULL_DISTANCE:
                return (supportedFeatures.shaderCullDistance == VK_TRUE);
            case SHADER_FLOAT64:
                return (supportedFeatures.shaderFloat64 == VK_TRUE);
            case SHADER_INT64:
                return (supportedFeatures.shaderInt64 == VK_TRUE);
            case SHADER_INT16:
                return (supportedFeatures.shaderInt16 == VK_TRUE);
            case SHADER_RESOURCE_RESIDENCY:
                return (supportedFeatures.shaderResourceResidency == VK_TRUE);
            case SHADER_RESOURCE_MIN_LOD:
                return (supportedFeatures.shaderResourceMinLod == VK_TRUE);
            case SPARSE_BINDING:
                return (supportedFeatures.sparseBinding == VK_TRUE);
            case SPARSE_RESIDENCY_BUFFER:
                return (supportedFeatures.sparseResidencyBuffer == VK_TRUE);
            case SPARSE_RESIDENCY_IMAGE_2D:
                return (supportedFeatures.sparseResidencyImage2D == VK_TRUE);
            case SPARSE_RESIDENCY_IMAGE_3D:
                return (supportedFeatures.sparseResidencyImage3D == VK_TRUE);
            case SPARSE_RESIDENCY_2_SAMPLES:
                return (supportedFeatures.sparseResidency2Samples == VK_TRUE);
            case SPARSE_RESIDENCY_4_SAMPLES:
                return (supportedFeatures.sparseResidency4Samples == VK_TRUE);
            case SPARSE_RESIDENCY_8_SAMPLES:
                return (supportedFeatures.sparseResidency8Samples == VK_TRUE);
            case SPARSE_RESIDENCY_16_SAMPLES:
                return (supportedFeatures.sparseResidency16Samples == VK_TRUE);
            case SPARSE_RESIDENCY_ALIASED:
                return (supportedFeatures.sparseResidencyAliased == VK_TRUE);
            case VARIABLE_MULTISAMPLE_RATE:
                return (supportedFeatures.variableMultisampleRate == VK_TRUE);
            case INHERITED_QUERIES:
                return (supportedFeatures.inheritedQueries == VK_TRUE);
            default:
                LN_ERR_FAIL_V_MSG("[Artemis] unkown feature!",false);
            }
            LN_ERR_FAIL_V_MSG("[Artemis] unkown feature!", false);

        }
        VkSurfaceCapabilitiesKHR device::surfaceCapabilities()
        {
            VkSurfaceCapabilitiesKHR temp;
            VkResult result = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &temp);
            LN_ERR_FAIL_COND_V_MSG(result != VK_SUCCESS, temp, "[Artemis] and error occured while trying to get device surface capabilities, VkResult: " + std::to_string(result));
            return temp;
        }
        VkQueue device::getQueue(vkb::QueueType type, bool dedicated)
        {
            return dedicated ? _device.get_dedicated_queue(type).value() : _device.get_queue(type).value();
        }
        uint32_t device::getQueueFamilyIndex(vkb::QueueType type, bool dedicated)
        {
            return dedicated ? _device.get_dedicated_queue_index(type).value() : _device.get_queue_index(type).value();
        }

        bool device::hasDedicatedQueue(vkb::QueueType type)
        {
            auto result = _device.get_dedicated_queue_index(type);
            return result.has_value();
        }
        ref<commandPool> device::getCommandPool(vkb::QueueType type, const VkCommandPoolCreateFlags createFlags)
        {
            //release the vkqueue!
            if(hasDedicatedQueue(type))
            {
                VkQueue queue = getQueue(type, true);
                if (queue != VK_NULL_HANDLE)
                    return ref<commandPool>(new commandPool(queue, getQueueFamilyIndex(type, true), createFlags, &_device.device));
            }
            return ref<commandPool>(new commandPool(getQueue(type, false),getQueueFamilyIndex(type,false),createFlags,&_device.device));
        }
        ref<semaphore> device::getSemaphore(const VkSemaphoreCreateFlags flags) const
        {
            return ref<semaphore>(new semaphore(&_device.device,flags));
        }
        ref<fence> device::getFence(const VkFenceCreateFlags flags) const
        {
            return ref<fence>(new fence(&_device.device,flags));
        }
        ref<swapchain> device::getSwapchain()
        {
            return ref<swapchain>(new swapchain(&_device,window->getWidth(),window->getHeight(), surfaceCapabilities().minImageCount + 1));
        }
        ref<sampler> device::getSampler(const VkFilter& filters, const VkSamplerAddressMode& samplerAddressMode)
        {
            return ref<sampler>(new sampler(&_device.device,filters,samplerAddressMode));
        }
        ref<allocator> device::getAllocator(const ref<commandPool> transferPool)
        {
            ref<commandPool> p_transferPool = transferPool == nullptr ? getCommandPool(vkb::QueueType::transfer) : transferPool;
            return ref<allocator>(new allocator(&_device.device,&instance.instance,&_device.physical_device.physical_device,_device.physical_device.properties.apiVersion,p_transferPool));
        }
        descriptorPoolBuilder device::getDescriptorPoolBuilder(const ref<shader> shader)
        {
            return descriptorPoolBuilder(&_device.device,shader);
        }
        pipelineBuilder device::getPipelineBuilder()
        {
            return pipelineBuilder(&_device.device);
        }
        renderPassBuilder device::getRenderPassBuilder()
        {
            return renderPassBuilder(&_device.device);
        }
        void device::waitIdle()
        {
            vkDeviceWaitIdle(_device);
        }
        VKAPI_ATTR VkBool32 VKAPI_CALL device::debugCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT           messageSeverity,
            VkDebugUtilsMessageTypeFlagsEXT                  messageTypes,
            const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
            void* pUserData)
        {
            switch (messageSeverity)
            {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                LN_CORE_TRACE("[Artemis] msg: \r\n {0}",pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                LN_CORE_INFO("[Artemis] msg: \r\n {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                LN_CORE_WARN("[Artemis] msg: \r\n {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                LN_CORE_ERROR("[Artemis] msg: \r\n {0}", pCallbackData->pMessage);
                break;
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_FLAG_BITS_MAX_ENUM_EXT:
                break;
            default:
                LN_CORE_CRITICAL("[Artemis] msg: \r\n {0}", pCallbackData->pMessage);
                break;
            }
            return VK_SUCCESS;
        };
    }
}
