#pragma once
#include <core/platform/windows/windowsWindow.h>
#define NULL_ATTACHEMENT_REF UINT32_MAX
namespace luna 
{
	namespace artemis 
	{
		class frameBuffer;
		class renderPass;
		class LN_API swapchain
		{
		public:
			void resize(uint32_t width, uint32_t height);
			const size_t size() { return m_swapchain.image_count; }
			~swapchain() { vkb::destroy_swapchain(m_swapchain); }
			_ALWAYS_INLINE_ VkViewport& getViewport();
			_ALWAYS_INLINE_ operator VkFormat() const { return m_swapchain.image_format; }
			_ALWAYS_INLINE_ operator VkSwapchainKHR() const { return m_swapchain.swapchain; }
			_ALWAYS_INLINE_ operator VkExtent2D() const { return m_swapchain.extent; }
			_ALWAYS_INLINE_ operator VkRect2D() const {
				VkRect2D renderArea;
				renderArea.extent = m_swapchain.extent;
				renderArea.offset = { 0,0 };
				return renderArea;
			}
			frameBuffer& getFrameBuffer(const ref<renderPass> p_renderPass,uint32_t index, const VkFramebufferCreateFlags flags, const uint32_t layers);
		protected:
			friend class device;
			swapchain(const vkb::Device* device, uint32_t width, uint32_t height, uint32_t imageCount);
		private:
			vkb::Swapchain m_swapchain;
			const vkb::Device* device = nullptr;
		};
	}
}


