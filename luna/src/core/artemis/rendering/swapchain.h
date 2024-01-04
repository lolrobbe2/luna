#pragma once
#include <core/platform/windows/windowsWindow.h>
#define NULL_ATTACHEMENT_REF UINT32_MAX
namespace luna 
{
	namespace artemis 
	{
		class swapchain
		{
		public:
			void resize(uint32_t width, uint32_t height);
			~swapchain() { vkb::destroy_swapchain(m_swapchain); }
			operator VkFormat() const { return m_swapchain.image_format; }
			operator VkSwapchainKHR() const { return m_swapchain.swapchain; }
			operator VkExtent2D() const { return m_swapchain.extent; }
		
		protected:
			friend class device;
			swapchain(const vkb::Device* device, uint32_t width, uint32_t height, uint32_t imageCount);
		private:
			vkb::Swapchain m_swapchain;
			const vkb::Device* device;
		};
	}
}


