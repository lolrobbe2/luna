#include "frameBuffer.h"
#include <core/debug/debugMacros.h>
#include "renderPass.h"
#include "image.h"
namespace luna 
{
	namespace artemis 
	{
		frameBuffer::frameBuffer(const VkDevice* device,const image& image, ref<renderPass> p_renderPass)
		{
			VkFramebufferCreateInfo info{ VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO };
			info.width = image.getExtent().x;
			info.height = image.getExtent().y;
			info.attachmentCount = 1;
			info.pAttachments = image.getViewPtr();
			info.layers = 1;
			info.renderPass = *p_renderPass;
			VkResult createRes = vkCreateFramebuffer(*device, &info, nullptr, &m_frameBuffer);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured whildt creating the frameBuffer, VkResult: " + VK_RESULT(createRes));
			m_info = info;

			VkDevice _device = (*device);
			this->device = _device;
		}
		frameBuffer::~frameBuffer()
		{
			
			//vkDestroyFramebuffer(device, m_frameBuffer, nullptr);
		}
		frameBuffer::frameBuffer(const VkDevice* device,const VkFramebufferCreateInfo& info)
		{
			VkResult createRes = vkCreateFramebuffer(*device, &info, nullptr,&m_frameBuffer);
			LN_ERR_FAIL_COND_MSG(createRes != VK_SUCCESS, "[Artemis] an error occured whildt creating the frameBuffer, VkResult: " + VK_RESULT(createRes));
			m_info = info;
		}
	}
}

