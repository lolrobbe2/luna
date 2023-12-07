 #include "renderPass.h"
#include <core/debug/debugMacros.h>
namespace luna
{
	namespace artemis 
	{
		renderPass::renderPass(const VkDevice* device,const VkRenderPassCreateInfo* info)
		{
			VkResult res = vkCreateRenderPass(*device, info, nullptr, &m_renderPass);
			LN_ERR_FAIL_COND_MSG(res != VK_SUCCESS, "an error occured when creating renderPass, VkResult: " + std::to_string(res));
			this->device = device;
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

		subPassBuilder subPassBuilder::setDepthStencilAttachement(VkImageLayout layout)
		{
			depthStencilAttachement = layout;
			return *this;
		}

		VkSubpassDescription subPassBuilder::build()
		{
			uint32_t attachement = 0;

			VkAttachmentReference* pInputAttachements = new VkAttachmentReference[inputAttachements.size()];
			for (size_t i = 0; i < inputAttachements.size(); i++)
			{
				pInputAttachements[i].attachment = attachement;
				pInputAttachements[i].layout = inputAttachements[i];
				attachement++;
			}
		
			VkAttachmentReference* pColorAttachements = new VkAttachmentReference[colorAttachements.size()];
			for (size_t i = 0; i < colorAttachements.size(); i++)
			{
				pColorAttachements[i].attachment = attachement;
				pColorAttachements[i].layout = colorAttachements[i];
				attachement++;
			}

			
			description.pInputAttachments = pInputAttachements;
			description.inputAttachmentCount = inputAttachements.size();

			description.pColorAttachments = pColorAttachements;
			description.colorAttachmentCount = colorAttachements.size();

			VkAttachmentReference _depthStencilAttachement{ ++attachement,depthStencilAttachement };
			description.pDepthStencilAttachment = &_depthStencilAttachement;

			return description;
		}

		renderPassBuilder renderPassBuilder::addSubPass(const VkSubpassDescription description)
		{
			subPasses.push_back(description);
			return *this;
		}

		renderPassBuilder renderPassBuilder::addAttachementDescription(const VkAttachmentDescription description)
		{
			attachementDescriptions.push_back(description);
			return *this;
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const VkSubpassDescription srcSubpass, const VkSubpassDescription dstSubpass, const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			VkSubpassDependency dependency;
			dependency.srcSubpass = std::find(subPasses.begin(), subPasses.end(), srcSubpass) - subPasses.begin();
			dependency.dstSubpass = std::find(subPasses.begin(), subPasses.end(), dstSubpass) - subPasses.begin();
			dependency.srcAccessMask = srcAccessMask;
			dependency.dstAccessMask = dstAccessMask;
			dependency.srcStageMask = srcStageMask;
			dependency.dstStageMask = dstStageMask;
			dependency.dependencyFlags = dependencyFlags;
			subpassDependencys.push_back(dependency);
			return *this;
		}

		renderPassBuilder renderPassBuilder::addSubPassDependency(const VkPipelineStageFlags srcStageMask, const VkPipelineStageFlags dstStageMask, const VkAccessFlags srcAccessMask, const VkAccessFlags dstAccessMask, const VkDependencyFlags dependencyFlags)
		{
			VkSubpassDependency dependency;
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = VK_SUBPASS_EXTERNAL;
			dependency.srcAccessMask = srcAccessMask;
			dependency.dstAccessMask = dstAccessMask;
			dependency.srcStageMask = srcStageMask;
			dependency.dstStageMask = dstStageMask;
			dependency.dependencyFlags = dependencyFlags;
			subpassDependencys.push_back(dependency);
			return *this;
		}

		renderPass renderPassBuilder::build()
		{
			VkRenderPassCreateInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};
			info.attachmentCount = attachementDescriptions.size();
			info.dependencyCount = subpassDependencys.size();
			info.subpassCount = subPasses.size();

			info.pAttachments = attachementDescriptions.data();
			info.pDependencies = subpassDependencys.data();
			info.pSubpasses = subPasses.data();
			return renderPass(device,&info);
		}

	}
}
