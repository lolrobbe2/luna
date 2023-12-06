#include "swapchain.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis 
	{
		void swapchain::resize(uint32_t width, uint32_t height)
		{
			uint32_t imageCount = m_swapchain.image_count;
			vkb::SwapchainBuilder builder{ *device };
			auto res = builder.set_old_swapchain(m_swapchain)
				.set_desired_extent(width, height)
				.build();
			LN_ERR_FAIL_COND_MSG(!res, "[Artemis] something went wrong when trying to create the swapchain, msg: " + res.error().message());
			vkb::destroy_swapchain(m_swapchain);
			m_swapchain = res.value();
		}
		swapchain::swapchain(const vkb::Device* device,uint32_t width,uint32_t height,uint32_t imageCount)
		{
			vkb::SwapchainBuilder builder{ *device };
			auto res = builder.add_fallback_present_mode(VK_PRESENT_MODE_FIFO_KHR)
				.set_desired_present_mode(VK_PRESENT_MODE_IMMEDIATE_KHR)
				.set_desired_format({ VK_FORMAT_B8G8R8A8_UNORM,VK_COLORSPACE_SRGB_NONLINEAR_KHR, })
				.set_desired_extent(width, height)
				.add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT)
				.set_required_min_image_count(imageCount)
				.build();
			LN_ERR_FAIL_COND_MSG(!res, "[Artemis] something went wrong when trying to create the swapchain, msg: " + res.error().message());
			m_swapchain = res.value();
		}
	}
}

