 #include "renderPass.h"
namespace luna
{
	namespace artemis 
	{
		renderPass::renderPass(const VkDevice* device)
		{
			VkRenderPassCreateInfo info;
			
			vkCreateRenderPass(*device, &info, nullptr, &m_renderPass);
		}

		subPassBuilder subPassBuilder::addInputAttachement(const VkImageLayout layout)
		{
			inputAttachements.push_back(layout);
			return *this;
		}

		subPassBuilder subPassBuilder::addColorAttachement(const VkImageLayout layout)
		{
			colorAttachements.push_back(layout);
			return *this;
		}

		subPassBuilder subPassBuilder::addDepthStencilAttachement(const VkImageLayout layout)
		{
			depthStencilAttachements.push_back(layout);
			return subPassBuilder();
		}

		VkSubpassDescription subPassBuilder::build()
		{
			uint32_t attachement;

			VkAttachmentReference* pInputAttachement = new VkAttachmentReference[inputAttachements.size()];
			for (size_t i = 0; i < inputAttachements.size(); i++)
			{
				pInputAttachement[i].attachment = attachement;
				pInputAttachement[i].layout = inputAttachements[i];
				attachement++;
			}
		
			VkAttachmentReference* pInputAttachement = new VkAttachmentReference[inputAttachements.size()];
			for (size_t i = 0; i < inputAttachements.size(); i++)
			{
				pInputAttachement[i].attachment = attachement;
				pInputAttachement[i].layout = inputAttachements[i];
				attachement++;
			}
			return VkSubpassDescription();
		}

	}
}
