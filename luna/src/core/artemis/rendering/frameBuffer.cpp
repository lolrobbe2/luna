#include "frameBuffer.h"
namespace luna 
{
	namespace artemis 
	{
		frameBuffer::frameBuffer(const VkDevice* device)
		{
			VkFramebufferCreateInfo info{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
			
			
			vkCreateFramebuffer(*device, &info, nullptr,&m_frameBuffer);
		}
	}
}

