#pragma once
#include <core/platform/windows/windowsWindow.h>
namespace luna 
{
	namespace artemis 
	{
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
		protected:
			friend class swapchain;
			frameBuffer(const VkDevice* device,const VkFramebufferCreateInfo& info);
		private:
			VkFramebufferCreateInfo m_info;
			VkFramebuffer m_frameBuffer = VK_NULL_HANDLE;
		};
	}
}


