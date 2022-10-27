#include "vulkanSwapchain.h"
#include <backends/imgui_impl_vulkan.h>
#include <core/vulkan/utils/vulkanAllocator.h>
namespace luna
{
	namespace vulkan
	{
		vulkanSwapchain::vulkanSwapchain(const swapchainSpec& swapChainSpec)
		{
			LN_CORE_TRACE("swapchain creation result = {0}",createSwapchain(swapChainSpec));
		}
		vulkanSwapchain::~vulkanSwapchain()
		{
			destroySwapchain();
		}
		VkResult vulkanSwapchain::createSwapchain(const swapchainSpec& swapChainSpec)
		{
            
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(swapChainSpec.physicalDevice, swapChainSpec.physicalDevice.surface, &surfaceCapaBilities);
            mSwapchainSpec = swapChainSpec;
            vkb::SwapchainBuilder swapchainBuilder{ swapChainSpec.physicalDevice, swapChainSpec.device, swapChainSpec.surface };
            mSwapchain = swapchainBuilder
                .use_default_format_selection()
                //use vsync present mode
                .set_desired_present_mode(VK_PRESENT_MODE_IMMEDIATE_KHR)
                //.set_desired_format({VK_FORMAT_R8G8B8A8_UNORM,VK_COLORSPACE_SRGB_NONLINEAR_KHR})
                .set_desired_format({ VK_FORMAT_B8G8R8A8_UNORM,VK_COLORSPACE_SRGB_NONLINEAR_KHR, })
                .set_desired_extent(swapChainSpec.window->getWidth(), swapChainSpec.window->getHeight())
                .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                .set_required_min_image_count(surfaceCapaBilities.minImageCount + 1)
                .build()
                .value();
          
            // std::vector<VkImageView> m_SwapChainImageViews; 
			return VK_SUCCESS;
		}
		VkResult vulkanSwapchain::recreateSwapchain()
		{
            vkDeviceWaitIdle(mSwapchainSpec.device);
            
            for (size_t i = 0; i < frameBuffers.size(); i++)
            {
                vkDestroyFramebuffer(mSwapchain.device, frameBuffers[i], nullptr);
            }
           
            vkb::SwapchainBuilder swapchainBuilder{ mSwapchainSpec.physicalDevice, mSwapchainSpec.device, mSwapchainSpec.surface };
            auto newSwapchain = swapchainBuilder.set_old_swapchain(mSwapchain)
                .use_default_format_selection()
                //use vsync present mode
                .set_desired_present_mode(VK_PRESENT_MODE_IMMEDIATE_KHR)
                .set_desired_format({ VK_FORMAT_B8G8R8A8_UNORM,VK_COLORSPACE_SRGB_NONLINEAR_KHR})
                .set_desired_extent(mSwapchainSpec.window->getWidth(), mSwapchainSpec.window->getHeight())
                .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
                .set_required_min_image_count(mSwapchain.image_count)
                .build();
            vkb::destroy_swapchain(mSwapchain);
            mSwapchain = newSwapchain.value();
			return VK_SUCCESS;
		}
		VkResult vulkanSwapchain::destroySwapchain()
		{
            if (mSwapchain == VK_NULL_HANDLE) return VK_ERROR_INVALID_EXTERNAL_HANDLE;
            for (size_t i = 0; i < frameBuffers.size(); i++)
            {
                utils::vulkanAllocator::destroyImageView(sceneViewportImageViews[i]);
                utils::vulkanAllocator::destroyImage(sceneViewportImages[i]);
                vkDestroyFramebuffer(mSwapchain.device, frameBuffers[i], nullptr);
            }
            mSwapchain.destroy_image_views(mSwapchain.get_image_views().value());
            vkDestroySwapchainKHR(mSwapchainSpec.device, mSwapchain, nullptr);
           
			return VK_SUCCESS;
		}

        VkViewport vulkanSwapchain::getViewport()
        {
            VkViewport viewport;
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = (float)mSwapchainSpec.window->getWidth();
            viewport.height = (float)mSwapchainSpec.window->getHeight();
            viewport.minDepth = 0.0f;
            viewport.maxDepth = 1.0f;
            return viewport;
        }

        VkRect2D vulkanSwapchain::getScissor()
        {
            VkRect2D scissor;
            scissor.extent = { mSwapchainSpec.window->getWidth(),mSwapchainSpec.window->getHeight() };
            scissor.offset = { 0,0 };
            return scissor;
        }
        VkResult vulkanSwapchain::initViewport()
        {
            if (init) return VK_SUCCESS;
            VkSamplerCreateInfo samplerInfo{};
            samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
            samplerInfo.magFilter = VK_FILTER_LINEAR;
            samplerInfo.minFilter = VK_FILTER_LINEAR;
            samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
            samplerInfo.anisotropyEnable = VK_FALSE;
            VkPhysicalDeviceProperties properties{};
            vkGetPhysicalDeviceProperties(mSwapchainSpec.physicalDevice, &properties);
            samplerInfo.maxAnisotropy = properties.limits.maxSamplerAnisotropy;
            samplerInfo.unnormalizedCoordinates = VK_FALSE;
            samplerInfo.compareEnable = VK_FALSE;
            samplerInfo.compareOp = VK_COMPARE_OP_ALWAYS;
            samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
            samplerInfo.mipLodBias = 0.0f;
            samplerInfo.minLod = 0.0f;
            samplerInfo.maxLod = 0.0f;

            sceneViewportImages.resize(mSwapchain.image_count);
            sceneViewportImageViews.resize(mSwapchain.image_count);
            for (size_t i = 0; i < mSwapchain.image_count; i++)
            {
                utils::vulkanAllocator::createImage(&sceneViewportImages[i], VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE,{ mSwapchain.extent.width,mSwapchain.extent.height,1 }, mSwapchain.image_format);
                utils::vulkanAllocator::createImageView(&sceneViewportImageViews[i], sceneViewportImages[i], mSwapchain.image_format, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT);
            }

            vkCreateSampler(mSwapchainSpec.device, &samplerInfo, nullptr, &viewportSampler);
            m_Dset.resize(mSwapchain.image_count);
            for (uint32_t i = 0; i < mSwapchain.image_count; i++)
                m_Dset[i] = ImGui_ImplVulkan_AddTexture(viewportSampler, sceneViewportImageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            init = true;
            return VK_SUCCESS;
        }
        VkResult vulkanSwapchain::recreateViewport()
        {
            sceneViewportImages.resize(mSwapchain.image_count);
            sceneViewportImageViews.resize(mSwapchain.image_count);
            for (size_t i = 0; i < mSwapchain.image_count; i++)
            {
                ImGui_ImplVulkan_RemoveTexture(m_Dset[i]);
                utils::vulkanAllocator::destroyImageView(sceneViewportImageViews[i]);
                utils::vulkanAllocator::destroyImage(sceneViewportImages[i]);
                utils::vulkanAllocator::createImage(&sceneViewportImages[i], VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, VMA_MEMORY_USAGE_AUTO_PREFER_DEVICE, { mSwapchain.extent.width,mSwapchain.extent.height,1 }, mSwapchain.image_format);
                utils::vulkanAllocator::createImageView(&sceneViewportImageViews[i], sceneViewportImages[i], mSwapchain.image_format, VkImageAspectFlagBits::VK_IMAGE_ASPECT_COLOR_BIT);
                m_Dset[i] = ImGui_ImplVulkan_AddTexture(viewportSampler, sceneViewportImageViews[i], VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            }
            return VK_SUCCESS;
        }
	}
}
