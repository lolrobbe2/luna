#include "frameBuffer.h"
#include <core/debug/debugMacros.h>
namespace luna 
{
	namespace artemis 
	{
		frameBuffer::frameBuffer(const VkDevice* device,const VkFramebufferCreateInfo& info)
		{
			VkResult createRes = vkCreateFramebuffer(*device, &info, nullptr,&m_frameBuffer);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured whildt creating the frameBuffer, VkResult: " + VK_RESULT(createRes));
			m_info = info;
		}
	}
}

