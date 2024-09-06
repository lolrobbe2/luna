#pragma once
#include <core/platform/windows/windowsWindow.h>

namespace luna 
{
	namespace artemis 
	{
		class image;
		class renderPass;
		class frameBuffer
		{
		public:
			frameBuffer() = default;
			_ALWAYS_INLINE_ operator VkRect2D() const {
				VkExtent2D extent;
				extent.width = m_info.width;
				extent.height = m_info.height;
				VkRect2D renderArea;
				renderArea.extent = extent;
				renderArea.offset = { 0,0 };
				return renderArea;
			}
			_ALWAYS_INLINE_ operator VkFramebuffer() const {
				return m_frameBuffer;
			}
			frameBuffer(const VkDevice* device,const image& image,ref<renderPass> p_renderPass);
			~frameBuffer();
		protected:
			friend class swapchain;
			frameBuffer(const VkDevice* device,const VkFramebufferCreateInfo& info);
		private:
			VkDevice device;
			VkFramebufferCreateInfo m_info = {};
			VkFramebuffer m_frameBuffer = VK_NULL_HANDLE;
		};
	}
}


